#include "planet.hpp"
#include "EasySDL/v6/EasySDL.hpp"

using namespace std;

#define WATER_BEACH (-100)
#define WATER_MED (-500)
#define WATER_DEEP (-3000)

/*void showShape(Shape s, int dX, int dY, int r=255, int g=255, int b=255)
{
 vector<Sint16> vx;
 vector<Sint16> vy;

 for (unsigned int i=0; i<s.points.size(); i++)
 {
  vx.push_back((Sint16)(s.exportPoint(i).x/20)+dX);
  vy.push_back((Sint16)(s.exportPoint(i).y/20)+dY);
 }

 filledPolygonRGBA(surface, &vx[0], &vy[0], vx.size(), r, g, b, 128);
}*/

#define MORECOLOR 0

void showHeight(int x, int y, int16_t h)
{
#if MORECOLOR
 int r=0, g=0, b=0;
 if (h>=0)
 {
  if (h<10)
  {
   r=0;
   g=100+87*h/500;
   b=70-2*h;
  }
  else if (h<500)
  {
   r=204*(h-10)/490;
   g=100+87*h/500;
   b=50+61*(h-10)/(500-10);
  }
  else if (h<2000)
  {
   r=204-93*(h-500)/(5000-500);
   g=187-187*(h-500)/(3000-500);
   b=111-111*(h-500)/(2000-500);
  }
  else if (h<3000)
  {
   r=204-93*(h-500)/(5000-500);
   g=187-187*(h-500)/(3000-500);
   b=0;
  }
  else if (h<5000)
  {
   r=204-93*(h-500)/(5000-500);
   g=111*(h-3000)/(5000-3000);
   b=111*(h-3000)/(5000-3000);
  }
  else if (h<9000)
  {
   r=111+144*(h-5000)/(4000);
   g=111+144*(h-5000)/(4000);
   b=111+144*(h-5000)/(4000);
  }
 }
 else
 {
  if (h>WATER_BEACH) r=255-h*255/WATER_BEACH;
  if (h>WATER_MED) g=255-h*255/WATER_MED;
  if (h>WATER_DEEP) b=255-h*255/WATER_DEEP;
 }
 window::place(draw::createRect(1, 1, r, g, b), true, x, y);
#else
 if (h<0)
 {
  int i=255/((-h)/100.0+1);
  window::place(draw::createRect(1, 1, 0, i/4, i), true, x, y);
 }
 else
 {
  int i=h*255/1000;
  int j=32/(h/20.0+1);
  if (i>255) i=255;
  window::place(draw::createRect(1, 1, i, i*1.1, i), true, x, y);
 }
#endif
}

void showSun(int x, int y, uint8_t s)
{
 window::place(draw::createRect(1, 1, s, s, s/2), true, x, y);
}

int xD=0, yD=0;
double dezoomFactor=2;

void showHeightMap(vector<vector<int16_t> > hm)
{
 for (int x=0; x<500; x++)
 {
  for (int y=0; y<500; y++)
  {
   showHeight(x, y, hm[(int(xD+x*dezoomFactor))%WORLD_SIZE][(int(yD+y*dezoomFactor))%WORLD_SIZE]);
  }
 }
 window::refresh();
 wait(30);
}

void (*showHM)(vector<vector<int16_t> >)=showHeightMap;

void showSunMap(vector<vector<uint8_t> > sm)
{
 for (int x=0; x<500; x++)
 {
  for (int y=0; y<500; y++)
  {
   showSun(x, y, sm[(int(xD+x*dezoomFactor))%WORLD_SIZE][(int(yD+y*dezoomFactor))%WORLD_SIZE]);
  }
 }
 window::refresh();
 waitFromLastWait(30);
}

void xPlus()
{
 xD+=20*dezoomFactor;
 if (xD>=WORLD_SIZE) xD-=WORLD_SIZE;
}

void xMinus()
{
 xD-=20*dezoomFactor;
 if (xD<0) xD+=WORLD_SIZE;
}

void yPlus()
{
 yD+=20*dezoomFactor;
 if (yD>=WORLD_SIZE) yD-=WORLD_SIZE;
}

void yMinus()
{
 yD-=20*dezoomFactor;
 if (yD<0) yD+=WORLD_SIZE;
}

void toggleZoom()
{
 dezoomFactor=2.25-dezoomFactor;
}

bool playing=false;
void play() {playing=!playing;}

#define disp(x) std::cout << #x << ":" << x << std::endl;

int main()
{
 uint64_t key;
 cout << "ready\n";
 cin >> key;
 World w1(key);

 initEasySDL(500, 500, "world n°"+to_string(key));
 associations::associateDown(SDLK_LEFT, xMinus);
 associations::associateDown(SDLK_RIGHT, xPlus);
 associations::associateDown(SDLK_UP, yMinus);
 associations::associateDown(SDLK_DOWN, yPlus);
 associations::associateDown(SDLK_SPACE, play);//toggleZoom);

 //w1.generate();

 w1.getSunRay(750, 900, 3, 0, 0);

 /**/int day=0;
 int time=0;
 int sec=0;
 w1.setSunMap(day, time, sec);
 cout << int(w1.sunMap[0][0]) << endl;
 while (!quit)
 {
  window::events();

  if (playing)
  {
   time+=1;
   if (time>=21)
   {
    day++;
    day%=42;
    time-=21;
   }
   w1.setSunMap(day, time, sec);
  }

  showSunMap(w1.sunMap);
 }/**/

 return 0;
}
