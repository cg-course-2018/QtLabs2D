#pragma once
#include "MeshP3N3.h"

namespace utils
{

// Генерирует список вершин отдельных треугольников, формирующих поверхность сферы.
// @param latitudeDivisions - число делений по широте, не менее 4
// @param longitudeDivisions - число делений по долготе, не менее 4
MeshDataP3N3 tesselateSphere(const Material &material, unsigned latitudeDivisions, unsigned longitudeDivisions);

// Генерирует список вершин отдельных треугольников, формирующих поверхность чайника.
// @param latitudeDivisions - число делений по широте, не менее 4
// @param longitudeDivisions - число делений по долготе, не менее 4
MeshDataP3N3 tesselateTeapot(const Material &material, unsigned latitudeDivisions, unsigned longitudeDivisions);

}
