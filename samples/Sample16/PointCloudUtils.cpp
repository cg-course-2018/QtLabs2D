#include "stdafx.h"
#include "PointCloudUtils.h"
#include "libplatform/ResourceLoader.h"
#include <glm/gtc/type_ptr.hpp>
#include <pcl/common/transforms.h>
#include <pcl/features/normal_3d.h>
#include <pcl/filters/voxel_grid.h>
#include <pcl/io/io.h>
#include <pcl/io/pcd_io.h>
#include <pcl/surface/concave_hull.h>
#include <pcl/surface/gp3.h>
#include <pcl/surface/marching_cubes_hoppe.h>
#include <pcl/surface/marching_cubes_rbf.h>
#include <pcl/surface/mls.h>

namespace utils
{
namespace
{
MeshDataP3C3N3 makeMeshDataWithTriangles(const Material &mat, const pcl::PointCloud<pcl::PointXYZRGBNormal>::Ptr &cloud, const pcl::PolygonMesh &triangleIndicies)
{
	MeshDataP3C3N3 data;
	data.primitive = gl::GL_TRIANGLES;
	data.material = mat;

	// Копируем индексы вершин в выходной массив индексов.
	data.indicies.reserve(3 * triangleIndicies.polygons.size());
	for (const pcl::Vertices &v : triangleIndicies.polygons)
	{
		if (v.vertices.size() != 3)
		{
			throw std::logic_error("triangulation produced non-triangle polygons with "
				+ std::to_string(v.vertices.size()) + " verticies");
		}
		data.indicies.insert(data.indicies.end(), v.vertices.begin(), v.vertices.end());
	}

	glm::vec3 color = { 1.0, 1.0, 1.0 };

	// Копируем данные вершин в выходной массив вершин.
	data.vertexes.reserve(cloud->points.size());
	for (const pcl::PointXYZRGBNormal &p : cloud->points)
	{

		data.vertexes.emplace_back(VertexP3C3N3{
			{ p.x, p.y, p.z },
			color,
			{ p.normal_x, p.normal_y, p.normal_z } });
	}

	return data;
}
} // namespace

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

pcl::PointCloud<pcl::PointXYZRGB>::Ptr makeVoxelGrid(const pcl::PointCloud<pcl::PointXYZRGB>::Ptr &cloud)
{
	// Применение VoxelGrid позволяет уменьшить число точек и одновременно распределить их более равномерно.

	constexpr float kLeafSize = 0.002f;
	pcl::PointCloud<pcl::PointXYZRGB>::Ptr merged(new pcl::PointCloud<pcl::PointXYZRGB>);
	*merged = *cloud;

	pcl::VoxelGrid<pcl::PointXYZRGB> voxelGrid;
	voxelGrid.setInputCloud(cloud);
	voxelGrid.setLeafSize(kLeafSize, kLeafSize, kLeafSize);
	voxelGrid.setDownsampleAllData(true);
	voxelGrid.filter(*merged);

	return merged;
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

pcl::PointCloud<pcl::PointXYZRGBNormal>::Ptr calculatePointCloudNormals2(const pcl::PointCloud<pcl::PointXYZRGB>::Ptr &cloud)
{
	// Радиус поиска - это максимальная длина ребра треугольника, то есть максимальное расстояние
	//  между двумя точками, которые ещё могут попасть в один треугольник.
	constexpr float kSearchRadius = 0.03f;

	// Составяем KD-дерево, используемое для поиска ближайших соседей каждой точки
	//  в процессе расчёта нормалей.
	pcl::search::KdTree<pcl::PointXYZRGB>::Ptr tree(new pcl::search::KdTree<pcl::PointXYZRGB>);
	// tree->setInputCloud(cloud);

	// Инициализируем выходное облако точек и объект, реализующий алгоритм Moving Least Squares
	pcl::PointCloud<pcl::PointXYZRGBNormal>::Ptr mlsPoints(new pcl::PointCloud<pcl::PointXYZRGBNormal>);
	pcl::MovingLeastSquares<pcl::PointXYZRGB, pcl::PointXYZRGBNormal> mls;
	mls.setComputeNormals(true);
	mls.setInputCloud(cloud);
	mls.setPolynomialFit(true);
	mls.setSearchMethod(tree);
	mls.setSearchRadius(kSearchRadius);

	// Реконструируем поверхность
	mls.process(*mlsPoints);

	return mlsPoints;
}

MeshDataP3C3N3 makeGreedyProjectionTriangulation(const Material &mat, const pcl::PointCloud<pcl::PointXYZRGBNormal>::Ptr &cloud)
{
	/*
	 * Основано на статье "Fast triangulation of unordered point clouds"
	 * http://pointclouds.org/documentation/tutorials/greedy_projection.php#greedy-triangulation
	 */

	// Радиус поиска - это максимальная длина ребра треугольника, то есть максимальное расстояние
	//  между двумя точками, которые ещё могут попасть в один треугольник.
	constexpr float kSearchRadius = 0.03f;

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
	pcl::PolygonMesh triangleIndicies;

	// Запускаем алгоритм реконструирования поверхности.
	gp3.setInputCloud(cloud);
	gp3.setSearchMethod(tree);
	gp3.reconstruct(triangleIndicies);

	return makeMeshDataWithTriangles(mat, cloud, triangleIndicies);
}

MeshDataP3C3N3 makeMarchingCubesTriangulation(const Material &mat, const pcl::PointCloud<pcl::PointXYZRGBNormal>::Ptr &cloud)
{
	constexpr float kGridResolution = 100.f;
	constexpr float kIsoLevel = 0.001f;
	constexpr float kExtendPercentage = 0.0f;

	// Класс PolygonMesh хранит индексы точек для каждого треугольника полигональной сетки.
	pcl::PolygonMesh triangleIndicies;

	// Класс MarchingCubesHoppe реализует одну из модификаций алгоритма Marching Cubes.
	pcl::MarchingCubesHoppe<pcl::PointXYZRGBNormal> mc;
	mc.setIsoLevel(kIsoLevel);
	mc.setGridResolution(kGridResolution, kGridResolution, kGridResolution);
	mc.setPercentageExtendGrid(kExtendPercentage);
	mc.setInputCloud(cloud);
	mc.reconstruct(triangleIndicies);

	return makeMeshDataWithTriangles(mat, cloud, triangleIndicies);
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
