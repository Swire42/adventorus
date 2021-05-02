#include "interpolate.hpp"

PointZdxy::PointZdxy(double _z, double _dx, double _dy)
{
 z=_z;
 dx=_dx;
 dy=_dy;
}

Interpolator::Interpolator(PointZdxy a, PointZdxy b, PointZdxy c, PointZdxy d)
{
 c00=a.z;
 c01=a.dy;
 c02=-3*a.z-2*a.dy+3*c.z-c.dy;
 c03=2*a.z+a.dy-2*c.z+c.dy;
 c10=a.dx;
 c11=b.z+b.dy+c.z+c.dx-a-a.dx-a.dy-d.z;
 c12=-c02-d.dy+3*d.z-3*b.z-2*b.dy;
 c13=b.dy-c03+d.dy-2*d.z+2*b.z;
 c20=-3*a.z-2*a.dx+3*b.z-b.dx;
 c21=-c20-d.dx+3*d.z-3*c.z-2*c.dx;
 c30=2*a+a.dx-2*b+b.dx;
 c31=c.dx-c30+d.dx-2*d.z+2*c.z;
}

double Interpolator::get(double x, double y)
{
 return c00+c01*y+c02*y*y+c03*y*y*y+c10*x+c11*x*y+c12*x*y*y+c13*x*y*y*y+c20*x*x+c21*x*x*y+c30*x*x*x+c31*x*x*x*y;
}
