#include "EasySDL/v6/EasySDL.hpp"

using namespace std;

#define SCALE (500.0/100)
#define RADIUS 5

struct Point
{
 double x,y;
};

int main()
{
 int nb;
 cin >> nb;

 vector<Point> p;
 for (int i=0; i<nb; i++)
 {
  p.push_back(Point());
  cin >> p.back().x >> p.back().y;
 }

 initEasySDL(500, 500);

 while (!quit)
 {
  window::events();

  for (unsigned int i=0; i<nb; i++)
  {
   filledCircleRGBA(surface, p[i].x*SCALE, p[i].y*SCALE, RADIUS*SCALE, 0, 128, 0, 128);
  }

  window::refresh();
  wait(30);
 }

 return 0;
}
