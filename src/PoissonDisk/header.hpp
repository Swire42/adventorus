#pragma once

#include <vector>

struct PDPoint
{
 double x, y;
 PDPoint(double _x, double _y);

 PDPoint generate(); // create a ramdom child
};

double sqDist(Point a, Point b); // squared dist

// Standard Poison-Disk (in opposition to Infinite Chunk-based Poison-Disk)
struct StPoisonDisk
{
 double radius, maxAttempts;
 vector<Point> points;
 vector<vector<int> > grid; // used to speed up distance checks.
 // an empty cell is equal to -1, a cell containing a point is equal to its
 // position in points.

 vector<int> active; // used to store all the active points. this store their position in points

 bool isFree(PDPoint); // check if we can add a point at this position

 void init(double w, double h); // set grid size and place first point randomly
 void compute(); // grow points until active is empty
};
