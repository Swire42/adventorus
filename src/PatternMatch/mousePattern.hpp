#pragma once

#include <iostream>
#include <vector>
#include <array>
#include <cmath>

#include "EasySDL/v6/EasySDL.hpp"
#include "Geometry/v2/geometry.hpp"

#include "order.hpp"

using namespace std;

const int nbSamples=20;

struct MousePattern
{
 bool recording=false;
 vector<GVector<2> > rawSampling;
 array<GVector<2>, nbSamples> constSampled;

 void clear();
 void reSample();

 double percentMatch(array<GVector<2>, nbSamples> comp);
 void startRec();
 Order validate();
 void addSample(int x, int y);
};

void initMousePatternDatabase();
