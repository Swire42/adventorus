#pragma once

#include "Geometry/v1/point.hpp"
#include <random>
#include "partchunk.hpp"

struct PDPoint
{
 Point2D pos;
 double radius;

 PDPoint(Point2D posi, double rad);
};

struct GridPos
{
 int x, y, attempts;

 GridPos();
 GridPos(int _x, int _y);
};

struct MaillageChunk
{
 uint64_t wSeed;
 ChunkPos pos;

 std::mt19937_64 mtGen;
 std::vector<PDPoint> points;
 std::vector<std::vector<int> > grid;
 std::vector<GridPos> available;

 int gridSize;
 double cellSize;
 double minRad, maxRad;

 MaillageChunk(uint64_t wS, ChunkPos p, double minR, double maxR);

 void init();

 PDPoint next();
 bool isFree(PDPoint p);
 void add(PDPoint p);

 void generate();
};
