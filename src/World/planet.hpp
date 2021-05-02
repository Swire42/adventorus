#pragma once

#include <vector>
//#include "continents.hpp"
#include "/home/victor/Programmation/C++/Headers/Geometry/v1/point.hpp"
#include "/home/victor/Programmation/C++/Headers/Geometry/v1/vector.hpp"

#include <cstdint>
#define WORLD_SIZE 1000

using namespace std;

struct World
{
 uint64_t key;
 //vector<Continent> conts;
 /*uint8_t loadedHM_x=0, loadedHM_y=0; // 0 to 9, part of the heightMap that is loaded.
 int16_t loadedHM[WORLD_SIZE/10][WORLD_SIZE/10]*/
 //int16_t heightMap[WORLD_SIZE][WORLD_SIZE];
 vector< vector<int16_t> > heightMap;
 vector< vector<uint8_t> > sunMap;
 vector< vector<uint8_t> > rainMap;

 World(uint64_t _key);

 // HeightMap funcs
 /*void loadHM(uint8_t _x, uint8_t _y);
 void writeHM();
 void changeHM(uint8_t _x, uint8_t _y);
 int16_t getHeight(uint16_t _x, uint16_t _y);
 void setHeight(uint16_t _x, uint16_t _y, int16_t value);*/

 void generate();

 /*void generateConts();

 void doContinentalDrift();
 void handleContsColisions();

 void initRelief();
 void applyContRelMods();

 void windErosion(int side);
 */
 void addNoises();
 void addNoise(int size, double coef, uint64_t aKey);

 // sun and spatial position : sunSpace.cpp
 void setSunMap(uint8_t day=0, uint8_t time=0, uint8_t sec=0);
 Vector3D_Co getSunRay(int x, int y, uint8_t day, uint8_t time, uint8_t sec); // NB : vector lenght = light strenght
 Point3D getSpacePos(int x, int y, uint8_t day, uint8_t time, uint8_t sec);
 double getAlphaRot(uint8_t day, uint8_t time, uint8_t sec);
 double getBetaRot(uint8_t day, uint8_t time, uint8_t sec);
 double getGammaRot(uint8_t day, uint8_t time, uint8_t sec, int x);
 double getDeltaRot(int y);

 void setWindMap();

 void setRainMap();

 void waterErosion();
};
