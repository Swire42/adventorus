#include <iostream>
#include <cmath>

using namespace std;

#define g 10.0
#define update_per_sec 10000.0

double trajectory(double x, double angle, double speed)
{
 // return the y value for the parameters specified
 return (-g/2)/(speed*speed*cos(angle)*cos(angle))*x*x+tan(angle)*x;
}

double angle(double x, double y, double speed)
{
 if (x<0)
 {
  cout << "calculating an angle for a negative value.\n";
  return 0;
 }
 double speed_2=speed*speed;
 double speed_4=speed_2*speed_2;
 double gxx=g*x*x;
 double ggxx=g*gxx;
 double part=-ggxx - 2*g*speed_2*y + speed_4;
 if (part<0)
 {
  return M_PI/4;
 }
 double solution=acos(1/(sqrt(2)*sqrt(
    (speed_4 / ggxx - (speed_2*y) / gxx)
    -((speed_2*sqrt(part)) / ggxx)
    )));
 if (y>=trajectory(x, 0, speed)) return solution;
 else return -solution;
}

struct Arrow
{
 double posx, posy, speedx, speedy;
 Arrow(double _posx, double _posy, double _speedx, double _speedy)
 {
  posx=_posx;
  posy=_posy;
  speedx=_speedx;
  speedy=_speedy;
 }

 Arrow(double speed, double angle)
 {
  posx=0;
  posy=0;
  speedx=cos(angle)*speed;
  speedy=sin(angle)*speed;
 }

 void update()
 {
  posx+=speedx/update_per_sec;
  posy+=speedy/update_per_sec;
  speedy-=(double)g/update_per_sec;
 }
};

double distance(double x1, double y1, double x2, double y2)
{
 return sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
}

int main()
{
 double px, py;
 cout << "x:";
 cin >> px;
 cout << "y:";
 cin >> py;
 cout << "---SIMULATING---\n";

 double anglePrec=0.1;

 for (int speed=20; speed<=100; speed+=10)
 {
  double worstDistance=0;
  int time=0;
  for (int angleMod=-1; angleMod<=1; angleMod++)
  {
   time=0;
   double minDist=1000;
   Arrow arrow=Arrow(speed, angle(px, py, speed)+anglePrec*angleMod);

   while (minDist>=distance(px, py, arrow.posx, arrow.posy))
   {
    minDist=distance(px, py, arrow.posx, arrow.posy);
    time++;
    arrow.update();
   }
   //cout << "(" << arrow.posx << ";" << arrow.posy << "): " << minDist*100 << "cm, " << (double)(time)/update_per_sec << "s\n";
   if (minDist>worstDistance) worstDistance=minDist;
  }
  cout << worstDistance*100 << "cm at " << speed << "m/s in " << time/update_per_sec << "s\n";
 }

 for (double precision=0; precision<=1; precision+=0.1)
 {
  cout << precision*100 << "% : " << sin((-199*precision)/2000+0.1)*px*100 << "cm\n";
 }
}
