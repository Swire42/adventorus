#include "partchunk.hpp"
#include <random>

#define NB_PARTS 1000
#define NB_CHUNKS 64

PartPos::PartPos(uint16_t _x, uint16_t _y) {x=_x; y=_y;}

PartPos PartPos::delta(int dx, int dy)
{
 return PartPos((x+dx)%NB_PARTS, (y+dy)%NB_PARTS);
}

ChunkPos::ChunkPos(PartPos _p, uint8_t _x, uint8_t _y) {p=_p; x=_x; y=_y;}
ChunkPos::ChunkPos(uint16_t px, uint16_t py, uint8_t cx, uint8_t cy) {p.x=px; p.y=py; x=cx; y=cy;}

ChunkPos ChunkPos::delta(int dx, int dy)
{
 int nx=x+dx;
 int ny=y+dy;
 return ChunkPos(p.delta(floor(double(nx)/NB_CHUNKS), floor(double(ny)/NB_CHUNKS)), nx%NB_CHUNKS, ny%NB_CHUNKS);
}

uint64_t genPartSeed(uint64_t wSeed, PartPos p)
{
 std::mt19937_64 mtGen;
 mtGen.seed(wSeed);
 uint64_t pos=p.y*0x10000+p.x;
 return mtGen()^pos;
}

uint64_t genChunkSeed(uint64_t wSeed, ChunkPos c)
{
 std::mt19937_64 mtGen;
 mtGen.seed(genPartSeed(wSeed, c.p));
 uint64_t pos=c.y*0x100+c.x;
 return mtGen()^pos;
}
