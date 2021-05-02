#pragma once

#include <array>

using namespace std;

enum {dirE, dirNE, dirN, dirNW, dirW, dirSW, dirS, dirSE};
extern array<string, 8> dirName;

enum {shOff=-2, shOn=-1};

enum {typeNone, typeShield, typeSimple, typeReverse, typeDouble, typeStrong, typeComplex};
extern array<string, 7> typeName;

struct Order
{
 int dir, type;
 Order() {dir=dirE; type=typeNone;}
 Order(int d, int t) {dir=d; type=t;}

 string name()
 {
  if (dir==-1) return typeName[type];
  else if (dir==shOff) return "Shield Off";
  else return dirName[dir]+" "+typeName[type];
 }
};

