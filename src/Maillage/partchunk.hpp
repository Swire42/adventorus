#pragma once

#include <cstdint>

struct PartPos
{
 uint16_t x, y;

 PartPos() {}
 PartPos(uint16_t _x, uint16_t _y);

 PartPos delta(int dx, int dy);
};

struct ChunkPos
{
 PartPos p;
 uint8_t x, y;

 ChunkPos() {}
 ChunkPos(PartPos _p, uint8_t _x, uint8_t _y);
 ChunkPos(uint16_t px, uint16_t py, uint8_t cx, uint8_t cy);

 ChunkPos delta(int dx, int dy);
};

uint64_t genPartSeed(uint64_t wSeed, PartPos p);

uint64_t genChunkSeed(uint64_t wSeed, ChunkPos c);
