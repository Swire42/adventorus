#include "planet.hpp"
#include <cmath>
#include <iostream>

#define disp(x) std::cout << #x << ":" << x << std::endl;

struct Ellipse
{
 double r, angle;
 Ellipse(double alpha, double beta);
 double distance(Point2D p, double x);
 double derSqDist(Point2D p, double x);
 double der2SqDist(Point2D p, double x);
};

Ellipse::Ellipse(double alpha, double beta)
{
 /// a) get two points to help us
 // first point is on the xy plane.
 double point1y=sin(alpha);
 // second point is the point with the highest z value.
 Point3D point2(0,1,0);
 point2.rotateAroundX(beta);

 point2.rotateAroundZ(alpha);
 if (point2.z<0) point2.scale(-1);

 if (point1y==0)
 {
  r=0;
  angle=atan2(point2.z, point2.y);
  return;
 }
 if (point2.z==0)
 {
  r=0;
  angle=0;
  return;
 }
 if (point2.y==0)
 {
  r=abs(point2.z);
  if ((point2.z) != (point2.x)) angle=M_PI;
  else angle=0;
  return;
 }

 /// b) get little radius of the ellipsoid
 double a=(point2.y*point2.y-point2.z*point2.z)/2;
 double b=point2.y*point2.z;
 double c=(point2.y*point2.y+point2.z*point2.z)/2;
 double p1y2=point1y*point1y;

 double theSqrt=sqrt(b*b * (p1y2-1) * p1y2*p1y2 * (a*a*p1y2 - a*a - 2*a*c*p1y2 + a*p1y2 + b*b*p1y2 - b*b + c*c*p1y2 + c*c - c*p1y2));
             // sqrt(b² * (p1y2-1) * p1y2² (a² p1y2 - a² - 2a c p1y2 + a p1y2 + b² p1y2 - b² + c² p1y2 + c² - c p1y2))
 double whatsLeft=-a*a*p1y2*p1y2 + 2*a*a*p1y2 + 2*a*c*p1y2*p1y2 - 2*a*c*p1y2 - a*p1y2*p1y2 -2*b*b*p1y2*p1y2 + 2*b*b*p1y2 - c*c*p1y2*p1y2 + c*p1y2*p1y2;
 double denominator=p1y2*p1y2*(a-c)*(a-c);
 double C=( 2*theSqrt + whatsLeft) / denominator;


 //double r1=1/sqrt(C1);
 r=1/sqrt(C);

 /// c) get rotation
 double cosi=(2 - p1y2 - p1y2*C) / (p1y2 - p1y2*C);

 angle=acos(cosi)/2;

 if (point2.y<0)
 {// max z is on the other side
  angle=M_PI-angle;
 }

 if (cos(alpha)*sin(alpha)>0)
 {
  angle+=M_PI;
 }

 Point2D point22d(point2.y, point2.z);
 point22d.rotate(-angle);
}

double Ellipse::distance(Point2D p, double x)
{
 return sqrt( (p.x-x)*(p.x-x) + (p.y-sqrt(1-x*x)*r)*(p.y-sqrt(1-x*x)*r) );
}

double Ellipse::derSqDist(Point2D p, double x)
{
 return x*(-2*r*r + 2 + (2*r*p.y) / sqrt(1 - x*x)) - 2*p.x;
}

double Ellipse::der2SqDist(Point2D p, double x)
{
 return -2*r*r + 2 + (2*r*p.y) / ((1-x*x)*sqrt(1-x*x));
}

double runNewton(Ellipse e, Point2D p, double x)
{// return an x position
 int i=0;
 bool positive=e.der2SqDist(p, x)>0;
 while (abs(e.derSqDist(p, x))>0.001)
 {
  if (positive!=(e.der2SqDist(p, x)>0)) break;
  x+=-e.derSqDist(p, x)/e.der2SqDist(p, x);
 }
 return x;
}

double runDumb(Ellipse e, Point2D p)
{
 double x=0;
 for (int i=-9; i<10; i++)
 {
  if (e.distance(p, 0.1*i)<e.distance(p, x)) x=0.1*i;
 }
 return x;
}

Vector3D_Co World::getSunRay(int x, int y, uint8_t day, uint8_t time, uint8_t sec)
{
 if ((y==0) && ((x%50)==0)) disp(x);
 /*** Step 1 : finding sun location in sky ***/

 Vector3D_Co sunRay(-1,0,0); // Sun = a long, long time ago, in a place far, far away on the x axis.

 // all angles are in radians
 double
  alpha=getAlphaRot(day, time, sec),
  beta=getBetaRot(day, time, sec),
  gamma=getGammaRot(day, time, sec, x),
  delta=getDeltaRot(y);

 sunRay.rotateAroundY(-delta);
 sunRay.rotateAroundZ(gamma);
 sunRay.rotateAroundX(beta);
 sunRay.rotateAroundZ(alpha);

 sunRay*=max(cos(Vector3D_Co(-1,0,0), sunRay), 0.0);

 /*** Step 2 : Self-shadowing ***/
 if (true)//(delta>M_PI/2) && (delta<3*M_PI/2) && (sunRay.lenght()!=0))
 {// area could be shadowed
  Ellipse ellipse(alpha, beta);

  //if (x+y==0) disp(ellipse.r)

  // geting point in 2D space
  Point3D curPoint3D=getSpacePos(x, y, day, time, sec);
  //if (curPoint3D.x>0) return Vector3D_Co(1,0,0);//sunRay;
  Point2D curPoint2D(curPoint3D.y, curPoint3D.z);
  curPoint2D.rotate(-ellipse.angle);
  Point3D center(1,0,0);
  center.rotateAroundZ(gamma);
  center.rotateAroundX(beta);
  center.rotateAroundZ(alpha);
  //if (center.x>0) return sunRay;
  //if (abs(curPoint2D.y)>0.6) return sunRay;

  // Geting distance
  // distance = min(minX, maxX, newtFromMin, newtFromCenter, newtFormMax)
  double best2dX=-1;
  double distance=dist(curPoint2D, Point2D(-1,0));
  if (dist(curPoint2D, Point2D(1,0))<distance)
  {
   best2dX=1;
   distance=dist(curPoint2D, Point2D(1,0));
  }
  for (int i=-1; i<=1; i++)
  {
   double x=runNewton(ellipse, curPoint2D, 0.99*i);
   double temp=ellipse.distance(curPoint2D, x);
   if (temp<distance)
   {
    best2dX=x;
    distance=temp;
   }
  }

  if (sin(acos(best2dX))*sin(acos(ellipse.r))<curPoint3D.x) return sunRay;

  double gradientSize=sin(acos(best2dX))*sin(acos(ellipse.r))*0.01;

  if (distance<=0.4-gradientSize)
  {
   /*if ((x==260) && (y==760))
   {//shall not be shadowed
    disp(sunRay.show())
    Vector3D_Co groundDir(1,0,0);
    groundDir.rotateAroundY(-delta);
    groundDir.rotateAroundZ(gamma);
    groundDir.rotateAroundX(beta);
    groundDir.rotateAroundZ(alpha);
    disp(groundDir.show())
    if (sunRay.lenght()!=0) sunRay=Vector3D_Co(1,0,0);
    disp(alpha)
    disp(beta)
    disp(curPoint3D.show())
    curPoint3D.rotateAroundZ(-alpha);
    curPoint3D.rotateAroundX(-beta);
    disp(curPoint3D.show())
    Point3D test(0.4,0,0);
    test.rotateAroundY(-delta);
    test+=Vector3D_Co(1,0,0);
    disp(test.show())
    disp(gamma)
    test.rotateAroundZ(gamma);
    disp(test.show())
    disp(ellipse.r)
    disp(ellipse.angle)
    disp(curPoint2D.show())
   }
   else*/
   sunRay*=0;
  }
  else if (distance<=0.4+gradientSize) sunRay*=(distance-0.4+gradientSize)*0.5/gradientSize;
  //if (distance<=0.4) sunRay=Vector3D_Co(1,0,0);
  //sunRay=Vector3D_Co(distance,0,0);
  //if (curPoint3D.x>0) {sunRay*=0.25; sunRay+=Vector3D_Co(0.25,0,0);}
 }
 return sunRay;
}

Point3D World::getSpacePos(int x, int y, uint8_t day, uint8_t time, uint8_t sec)
{
 Point3D p(0.4, 0, 0);

 // all angles are in radians
 double
  alpha=getAlphaRot(day, time, sec),
  beta=getBetaRot(day, time, sec),
  gamma=getGammaRot(day, time, sec, x),
  delta=getDeltaRot(y);

 p.rotateAroundY(-delta);
 p+=Vector3D_Co(1,0,0);
 p.rotateAroundZ(gamma);
 p.rotateAroundX(beta);
 p.rotateAroundZ(alpha);

 return p;
}

double World::getAlphaRot(uint8_t day, uint8_t time, uint8_t sec)
{
 return 2*M_PI*(day/42.0 + time/42.0/21 + sec/42.0/21/60);
}

double World::getBetaRot(uint8_t day, uint8_t time, uint8_t sec)
{
 return M_PI/4;
}

double World::getGammaRot(uint8_t day, uint8_t time, uint8_t sec, int x)
{
 return (time/21.0 + sec/21.0/60 + double(x)/WORLD_SIZE) *2*M_PI;
}

double World::getDeltaRot(int y)
{
 return y*2*M_PI/WORLD_SIZE;
}

