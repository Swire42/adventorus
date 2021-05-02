#pragma once

// Land parts are zone of 1*1km
// a land part contains 4096 chunks
struct LandPart
{
 vector<vector<double> > heightMap;

 void generate();
};
