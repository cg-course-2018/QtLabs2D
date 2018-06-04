#include "stdafx.h"
#include "PointCloudUtils.h"
#include "libplatform/ResourceLoader.h"
#include <glm/gtc/type_ptr.hpp>
#include <pcl/common/transforms.h>
#include <pcl/features/normal_3d.h>
#include <pcl/io/io.h>
#include <pcl/io/pcd_io.h>
#include <pcl/surface/concave_hull.h>
#include <pcl/surface/gp3.h>

namespace utils
{

pcl::PointCloud<pcl::PointXYZRGB>::Ptr loadPointCloud(const std::string &relativePath)
{
	const std::string absolutePath = platform::ResourceLoader::getAbsolutePath(relativePath);

	pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZRGB>);
	if (pcl::io::loadPCDFile(absolutePath, *cloud) == -1)
	{
		throw std::runtime_error("cannot read point cloud from " + relativePath);
	}

	return cloud;
}

pcl::PointCloud<pcl::PointXYZRGB>::Ptr applyTransform(const pcl::PointCloud<pcl::PointXYZRGB>::Ptr &cloud, const glm::mat4 &transform)
{
	/*
	 * Основано на статье "Using a matrix to transform a point cloud"
	 * http://pointclouds.org/documentation/tutorials/matrix_transform.php#matrix-transform
	 */

	float values[16] = { 0 };
	memcpy(values, glm::value_ptr(transform), sizeof(values));
	Eigen::Matrix4f eigenTransform = Eigen::Matrix4f(values);

	pcl::PointCloud<pcl::PointXYZRGB>::Ptr transformedCloud(new pcl::PointCloud<pcl::PointXYZRGB>());
	pcl::transformPointCloud(*cloud, *transformedCloud, eigenTransform);

	return transformedCloud;
}

pcl::PointCloud<pcl::PointXYZRGB>::Ptr makeConcaveHull(const pcl::PointCloud<pcl::PointXYZRGB>::Ptr &cloud)
{
	// Параметр alpha ограничивает размер сегментов невыпуклой оболочки.
	// Чем меньше alpha, тем детальнее будет выпуклая оболочка.
	constexpr float alpha = 0.1f;

	pcl::PointCloud<pcl::PointXYZRGB>::Ptr result(new pcl::PointCloud<pcl::PointXYZRGB>);
	pcl::ConcaveHull<pcl::PointXYZRGB> concaveHull;
	concaveHull.setInputCloud(cloud);
	concaveHull.setAlpha(alpha);
	concaveHull.reconstruct(*result);

	return result;
}

pcl::PointCloud<pcl::PointXYZRGBNormal>::Ptr calculatePointCloudNormals(const pcl::PointCloud<pcl::PointXYZRGB>::Ptr &cloud)
{
	// Создаём облако точек, в которое будут записаны нормали.
	pcl::PointCloud<pcl::Normal>::Ptr normals(new pcl::PointCloud<pcl::Normal>);

	// Составяем KD-дерево, используемое для поиска ближайших соседей каждой точки
	//  в процессе расчёта нормалей.
	pcl::search::KdTree<pcl::PointXYZRGB>::Ptr tree(new pcl::search::KdTree<pcl::PointXYZRGB>);
	tree->setInputCloud(cloud);

	// Запускаем вычисление оценки нормалей
	pcl::NormalEstimation<pcl::PointXYZRGB, pcl::Normal> estimation;
	estimation.setInputCloud(cloud);
	estimation.setSearchMethod(tree);
	estimation.setKSearch(20);
	estimation.compute(*normals);

	pcl::PointCloud<pcl::PointXYZRGBNormal>::Ptr cloudWithNormals(new pcl::PointCloud<pcl::PointXYZRGBNormal>);
	pcl::concatenateFields(*cloud, *normals, *cloudWithNormals);

	return cloudWithNormals;
}

MeshDataP3C3N3 makeGreedyProjectionTriangulation(const Material &mat, const pcl::PointCloud<pcl::PointXYZRGBNormal>::Ptr &cloud)
{
	/*
	 * Основано на статье "Fast triangulation of unordered point clouds"
	 * http://pointclouds.org/documentation/tutorials/greedy_projection.php#greedy-triangulation
	 */

	// Радиус поиска - это максимаьная длина ребра треугольника, то есть максимальное расстояние
	//  между двумя точками, которые ещё могут попасть в один треугольник.
	constexpr float kSearchRadius = 0.025f;

	// Максимальное число соседей - это максимальное число соседних точек, среди которых выбираются
	//  точки для составления треугольников восстановленной поверхности.
	// Типичное значение: от 50 до 100.
	constexpr float kMaximumNearestNeighbors = 100;

	// Параметр Mu определяет максимальный разброс между расстоянием до ближайшей точки и
	//  расстоянием до самой удалённой точки из набора ближайших точек.
	// Типичное значение: от 2.5 до 3, либо 1.5 для получения равномерной сетки.
	constexpr float kMuParameter = 2.5f;

	// Минимальный и максимальный углы в треугольнике, полученном при триангуляции.
	// Типичные значения 10° и 120° соответственно.
	constexpr float kMinTriangleAngle = static_cast<float>(M_PI / 18);
	constexpr float kMaxTriangleAngle = static_cast<float>(2 * M_PI / 3);

	// Максимальный угол между смежными треугольниками, может быть увеличен, если у облака
	//  точек предполагаются чётко выделенные углы.
	constexpr float kMaxSurfaceAngle = static_cast<float>(M_PI / 4);

	// Алгоритм GreedyProjectionTriangulation использует нормаль к точке для выбора точек соединения
	//  среди ближайших соседей. Ближайшие соседи (nearest neighbours) проецируются на нормаль,
	//  а затем проекция используеся для выбора соседа, к которому надо провести ребро треугольника.
	pcl::GreedyProjectionTriangulation<pcl::PointXYZRGBNormal> gp3;
	gp3.setSearchRadius(kSearchRadius);
	gp3.setMu(kMuParameter);
	gp3.setMaximumNearestNeighbors(kMaximumNearestNeighbors);
	gp3.setMinimumAngle(kMinTriangleAngle);
	gp3.setMaximumAngle(kMaxTriangleAngle);
	gp3.setMaximumSurfaceAngle(kMaxSurfaceAngle);
	gp3.setNormalConsistency(false);

	// Составяем KD-дерево, используемое для поиска ближайших соседей каждой точки
	//  в процессе расчёта нормалей.
	pcl::search::KdTree<pcl::PointXYZRGBNormal>::Ptr tree(new pcl::search::KdTree<pcl::PointXYZRGBNormal>);
	tree->setInputCloud(cloud);

	// Класс PolygonMesh хранит индексы точек для каждого треугольника полигональной сетки.
	pcl::PolygonMesh triangles;

	// Запускаем алгоритм реконструирования поверхности.
	gp3.setInputCloud(cloud);
	gp3.setSearchMethod(tree);
	gp3.reconstruct(triangles);

	MeshDataP3C3N3 data;
	data.primitive = gl::GL_TRIANGLES;
	data.material = mat;

	// Копируем индексы вершин в выходной массив индексов.
	data.indicies.reserve(3 * triangles.polygons.size());
	for (const pcl::Vertices &v : triangles.polygons)
	{
		if (v.vertices.size() != 3)
		{
			throw std::logic_error("triangulation produced non-triangle polygons with "
				+ std::to_string(v.vertices.size()) + " verticies");
		}
		data.indicies.insert(data.indicies.end(), v.vertices.begin(), v.vertices.end());
	}

	// Копируем данные вершин в выходной массив вершин.
	data.vertexes.reserve(cloud->points.size());
	for (const pcl::PointXYZRGBNormal &p : cloud->points)
	{
		data.vertexes.emplace_back(VertexP3C3N3{
			{ p.x, p.y, p.z },
			{ p.r, p.g, p.b },
			{ p.normal_x, p.normal_y, p.normal_z } });
	}

	return data;
}

MeshDataP3C3N3 makeMeshFromPoints(const Material &mat, const pcl::PointCloud<pcl::PointXYZRGBNormal>::Ptr &cloud)
{
	MeshDataP3C3N3 data;
	data.primitive = gl::GL_POINTS;
	data.material = mat;

	// Копируем данные вершин в выходной массив вершин.
	data.vertexes.reserve(cloud->points.size());
	for (const pcl::PointXYZRGBNormal &p : cloud->points)
	{
		data.vertexes.emplace_back(VertexP3C3N3{
			{ p.x, p.y, p.z },
			{ p.r, p.g, p.b },
			{ p.normal_x, p.normal_y, p.normal_z } });
	}

	// Массив индексов будет просто перечислять вершины.
	data.indicies.resize(data.vertexes.size());
	std::iota(data.indicies.begin(), data.indicies.end(), 0);

	return data;
}

} // namespace utils
