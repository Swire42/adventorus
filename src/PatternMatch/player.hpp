#pragma once

#include <iostream>

#include "order.hpp"
#include "EasySDL/v6/EasySDL.hpp"
#include "Geometry/v2/geometry.hpp"

using namespace std;

struct Player
{
 string name;
 GPoint<2> swordPos;
 Order curOrder, nextOrder;
 int phase;
 int shieldDuration;
 bool shield;
 int atkCooldown;
 int life;
 Player *enemy;
 SDL_Surface *swordSurf, *shieldSurf;

 Player(string n);

 void pushOrder(Order o);
 void tick();
 void display();
 void recieveDamage(int d);
 bool isDead();
};
