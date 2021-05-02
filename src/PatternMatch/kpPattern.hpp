#pragma once

#include <vector>

#include "order.hpp"

using namespace std;

const int kpTimeout=150;

struct KPPattern
{
 bool recording=false;
 int lastEv;
 vector<int> history;

 void input(int dir);
 bool isFinished();
 Order get();
};
