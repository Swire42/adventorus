#include "Coords/v2/Coords.hpp"
#include "MyMath/v2/MyMath.hpp"
#include "EasySDL/v6/EasySDL.hpp"
#include <vector>
#include <random>

#include "maillage.hpp"

using namespace std;

#define SCALE 500

int main()
{
 uint64_t wSeed;
 cin >> wSeed;

 initEasySDL(SCALE*2, SCALE);

 MaillageChunk ch(wSeed, ChunkPos(10, 10, 10, 10), 0.02, 0.02);
 ch.generate();
 MaillageChunk ch2(wSeed, ChunkPos(10, 10, 11, 10), 0.02, 0.02);
 ch2.generate();

 while (!quit)
 {
  window::events();

  window::place(draw::createRect(SCALE*2, SCALE, 255, 255, 255), true, 0, 0);

  /*for (unsigned int x=0; x<ch.gridSize; x++)
  for (unsigned int y=0; y<ch.gridSize; y++)
  {
   srand(ch.grid[x][y]);
   window::place(draw::createRect(ch.cellSize*SCALE, ch.cellSize*SCALE, rand()%255, rand()%255, rand()%255), true, x*ch.cellSize*SCALE, y*ch.cellSize*SCALE);
  }*/

  for (unsigned int i=0; i<ch.points.size(); i++)
  {
   //aacircleRGBA(surface, ch.points[i].pos.x*SCALE, ch.points[i].pos.y*SCALE, ch.points[i].radius*SCALE, 255, 0, 0, 128);
   //filledCircleRGBA(surface, ch.points[i].pos.x*SCALE, ch.points[i].pos.y*SCALE, ch.points[i].radius*SCALE, 0, 0, 255, 128);
   filledCircleRGBA(surface, ch.points[i].pos.x*SCALE, ch.points[i].pos.y*SCALE, 1, 0, 0, 0, SDL_ALPHA_OPAQUE);
  }
  for (unsigned int i=0; i<ch2.points.size(); i++)
  {
   //aacircleRGBA(surface, ch.points[i].pos.x*SCALE, ch.points[i].pos.y*SCALE, ch.points[i].radius*SCALE, 255, 0, 0, 128);
   //filledCircleRGBA(surface, ch2.points[i].pos.x*SCALE+SCALE, ch2.points[i].pos.y*SCALE, ch2.points[i].radius*SCALE, 0, 0, 255, 128);
   filledCircleRGBA(surface, ch.points[i].pos.x*SCALE+SCALE, ch.points[i].pos.y*SCALE, 1, 0, 0, 0, SDL_ALPHA_OPAQUE);
  }

  SDL_SaveBMP(surface, "out.bmp");
  break;

  window::refresh();
  wait(30);
 }


 return 0;
}
