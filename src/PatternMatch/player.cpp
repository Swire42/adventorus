#include "player.hpp"

using namespace std;

double const speed[3]={0.1, 0.2, 0.05};
int const atkCooldownDur=10;

Player::Player(string n)
{
 name=n;

 image::load(name+"_sword.png", &swordSurf);
 image::load(name+"_shield.png", &shieldSurf);

 swordPos = GPoint<2>({0,0});
 phase=0;
 shieldDuration=0;
 life=100;
}

void Player::pushOrder(Order o)
{
 cout << name << ":" << o.name() << endl;
 if (o.type==typeSimple) nextOrder = o;
}

GPoint<2> atckAim(int dir)
{
 if (dir==dirE) return GPoint<2>({1,0});
 else if (dir==dirNE) return GPoint<2>({0.707,0.707});
 else if (dir==dirN) return GPoint<2>({0,1});
 else if (dir==dirNW) return GPoint<2>({-0.707,0.707});
 else if (dir==dirW) return GPoint<2>({-1,0});
 else if (dir==dirSW) return GPoint<2>({-0.707,-0.707});
 else if (dir==dirS) return GPoint<2>({0,-1});
 else if (dir==dirSE) return GPoint<2>({0.707,-0.707});
 else return GPoint<2>({0,0});
}

GPoint<2> prepAim(int dir)
{
 if (dir==dirE) return GPoint<2>({-1,0});
 else if (dir==dirNE) return GPoint<2>({-0.707,-0.707});
 else if (dir==dirN) return GPoint<2>({0,-1});
 else if (dir==dirNW) return GPoint<2>({0.707,-0.707});
 else if (dir==dirW) return GPoint<2>({1,0});
 else if (dir==dirSW) return GPoint<2>({0.707,0.707});
 else if (dir==dirS) return GPoint<2>({0,1});
 else if (dir==dirSE) return GPoint<2>({-0.707,0.707});
 else return GPoint<2>({0,0});
}

void Player::tick()
{
 if (phase==2)
 {
  phase=0;
  atkCooldown=atkCooldownDur;
  curOrder.type=typeNone;
 }
 if (curOrder.type == typeNone && atkCooldown==0)
 {
  curOrder=nextOrder;
  nextOrder.type=typeNone;
 }
 else if (atkCooldown>0) atkCooldown--;
 if (curOrder.type != typeNone)
 {
  GPoint<2> aim;
  if (phase==0) aim=prepAim(curOrder.dir);
  else aim=atckAim(curOrder.dir);

  GVector<2> tr=aim-swordPos;
  double l=tr.len();
  if (l<speed[phase])
  {
   swordPos=aim;
   phase++;
  }
  else
  {
   tr*=speed[phase]/l;
   if(dist(GPoint<2>({0,0}), swordPos)<speed[1]/2 && phase==1) enemy->recieveDamage(10); 
   swordPos+=tr;
  }
 }
 else
 {
  GPoint<2> aim({0,0});
  GVector<2> tr=aim-swordPos;
  double l=tr.len();
  if (l<speed[2])
  {
   swordPos=aim;
  }
  else
  {
   tr*=speed[2]/l;
   swordPos+=tr;
  }
 }
}

void Player::display()
{
 if (shield) window::place(shieldSurf, false, surface->w/2, surface->h/2, x_center, y_middle);

 window::place(swordSurf, false, swordPos[0]*surface->w/2.5+surface->w/2, -swordPos[1]*surface->h/2.5+surface->h/2, x_center, y_middle, curOrder.dir*45);
}

void Player::recieveDamage(int d)
{
 life-=d;
 if (isDead()) std::cout << name << " is FUCKIN' DEAD !!\n";
}

bool Player::isDead()
{
 return life<=0;
}
