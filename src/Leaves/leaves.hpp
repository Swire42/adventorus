#pragma once

#include "Geometry/v1/point.hpp"

struct Leave
{
 Point2D A, B;
 double size;
 double r, g, b;

 Leave(uint64_t key);
};
