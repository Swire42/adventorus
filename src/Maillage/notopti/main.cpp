#include "Coords/v2/Coords.hpp"
#include "MyMath/v2/MyMath.hpp"
#include "EasySDL/v6/EasySDL.hpp"
#include <vector>
#include <random>

#include "maillage.hpp"

using namespace std;

#define SCALE 1000

int main()
{
 uint64_t wSeed;
 cin >> wSeed;

 initEasySDL(SCALE, SCALE);

 MaillageChunk ch(wSeed, ChunkPos(10, 10, 10, 10), 0.01, 0.05);
 ch.generate();

 while (!quit)
 {
  window::events();

  window::place(draw::createRect(ch.cellSize*SCALE, ch.cellSize*SCALE, 255, 255, 255), true, 0, 0);

  for (unsigned int i=0; i<ch.points.size(); i++)
  {
   filledCircleRGBA(surface, ch.points[i].pos.x*SCALE, ch.points[i].pos.y*SCALE, ch.points[i].radius*SCALE, 255, 255, 255, 128);
  }

  window::refresh();
  wait(30);
 }

 return 0;
}
