#include "EasySDL/v6/EasySDL.hpp"
#include "leaves.hpp"

SDL_Surface* showLeave(uint64_t seed)
{
 SDL_Surface* surf=draw::createRect(100, 100, 255, 255, 255);
 Leave l(seed);

 vector<Sint16> vx;
 vector<Sint16> vy;

 vx.push_back(50);
 vy.push_back(90);

 vx.push_back(50-l.A.y*80);
 vy.push_back(90-l.A.x*80);

 vx.push_back(50-l.B.y*80);
 vy.push_back(90-l.B.x*80);

 vx.push_back(50);
 vy.push_back(10);

 filledPolygonRGBA(surf, &vx[0], &vy[0], vx.size(), l.r, l.g, l.b, SDL_ALPHA_OPAQUE);
 aapolygonRGBA(surf, &vx[0], &vy[0], vx.size(), l.r, l.g, l.b, SDL_ALPHA_OPAQUE);

 vx[1]=100-vx[1];
 vx[2]=100-vx[2];

 filledPolygonRGBA(surf, &vx[0], &vy[0], vx.size(), l.r*0.9, l.g*0.9, l.b*0.9, SDL_ALPHA_OPAQUE);
 aapolygonRGBA(surf, &vx[0], &vy[0], vx.size(), l.r*0.9, l.g*0.9, l.b*0.9, SDL_ALPHA_OPAQUE);

 return surf;
}

int seed=0;

void next()
{
 seed++;
}

int main()
{
 initEasySDL(100, 100);

 associations::associateDown(SDLK_SPACE, next);

 for (unsigned int i=0; i<400; i++)
 {
  string file="img/"+to_string(i%10000/1000)+to_string(i%1000/100)+to_string(i%100/10)+to_string(i%10)+".bmp";
  SDL_Surface* surf=showLeave(i+2569833);
  SDL_SaveBMP(surf, file.c_str());
  SDL_FreeSurface(surf);
 }
 return 0;
 while (true)
 {
  window::events();
  if (quit) break;

  window::place(showLeave(seed), true, 0, 0);

  waitFromLastWait(30);
  window::refresh();
 }
}
