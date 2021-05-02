#pragma once

struct PointZdxy
{
 double z, dx, dy;

 PointZdxy(double _z, double _dx, double _dy);
}

struct Interpolator
{
 double c00, c01, c02, c03, c10, c11, c12, c13, c20, c21, c30, c31;

 // C--D
 // |  |
 // A--B
 Interpolator(PointZdxy _a, PointZdxy _b, PointZdxy _c, PointZdxy _d);
 double get(double x, double y);
};
