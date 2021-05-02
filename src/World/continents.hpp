#ifndef CONTINENTS_HPP
#define CONTINENTS_HPP

#include <random>
#include "Coords/v2/Coords.hpp"
#include "Coords/v2/Shape.hpp"

using namespace std;

#define WORLD_SIZE (1000)

#define PANGEA_NB_SEGS (500)
#define PANGEA_SEG0_DIST (int(WORLD_SIZE*0.3))
#define PANGEA_MAX_DELTA (WORLD_SIZE/100)
#define PANGEA_JOIN_DELTA (PANGEA_MAX_DELTA/4)
#define PANGEA_MAX_SEG_DIST (int(WORLD_SIZE*0.35))
#define PANGEA_MIN_SEG_DIST (int(WORLD_SIZE*0.25))

#define CONTINENTS_CUT_SEG_LEN (WORLD_SIZE/200)

struct ContRelMod;

struct Continent
{
 Shape shape;
 CoordVector movement;
 double mvtRotSpeed;
 double rotSpeed;
 vector<ContRelMod> contRelMods;
 vector<ContRelMod> newContRelMods;

 Continent();

 Continent split(mt19937_64* mtGen);
 void move();
};

struct Pangea
{
 int seg[PANGEA_NB_SEGS];

 Pangea(mt19937_64* mtGen);
 void generate(mt19937_64* mtGen);
 Continent exportCont();
 operator Continent();
};

#define CONTRELMOD_NONE -1
#define CONTRELMOD_COLISION 1
#define CONTRELMOD_OCEANPUSHUP 2

struct ContRelMod // ContinentReliefModifier
{
 Coord position;
 double power;
 int type;

 ContRelMod();
 ContRelMod(Coord pos, double pow, int t);
};



void handlePotColision(Continent* a, Continent* b);

#endif // CONTINENTS_HPP
