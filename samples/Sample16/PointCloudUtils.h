#pragma once

#include "MeshP3C3N3.h"
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>

namespace utils
{

pcl::PointCloud<pcl::PointXYZRGB>::Ptr loadPointCloud(const std::string &relativePath);
pcl::PointCloud<pcl::PointXYZRGBNormal>::Ptr calculatePointCloudNormals(const pcl::PointCloud<pcl::PointXYZRGB>::Ptr &cloud);

MeshDataP3C3N3 makeGreedyProjectionTriangulation(const Material &mat, const pcl::PointCloud<pcl::PointXYZRGBNormal>::Ptr &cloud);

} // namespace utils
