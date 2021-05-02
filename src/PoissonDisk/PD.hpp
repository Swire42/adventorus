#pragma once

#include <random>
#include <vector>

struct Coord
{
 double x, y;
 Coord(double _x, double _y);
};

struct PDPoint
{
 Coord p;
 std::mt19937_64 mtGen;
 
 PDPoint(Coord _p); // set up the mt generator

 std::vector<Coord> generate(int k); // generate k random childs
};


// Standard Poison-Disk (in opposition to Infinite Chunk-based Poison-Disk)
struct StPoisonDisk
{
 double radius, maxAttempts;
 std::vector<PDPoint> points;
 std::vector<std::vector<int> > grid; // used to speed up distance checks.
 // an empty cell is equal to -1, a cell containing a point is equal to its
 // position in points.

 std::vector<int> active; // used to store all the active points. this store their position in points

 bool isFree(PDPoint p, int gridSampleLin, int gridSampleCol); // check if we can add a point at this position

 void init(); // set grid size and place first point randomly
 void compute(); // grow points until active is empty
};
