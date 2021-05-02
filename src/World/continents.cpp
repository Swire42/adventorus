#include "continents.hpp"
#include "MyMath/v2/MyMath.hpp"
#include "Coords/v2/Coords.hpp"
#include "Coords/v2/Shape.hpp"
#include <iostream>

using namespace std;

#define CRM_COL_COEF (50000)

Continent::Continent()
{
 mvtRotSpeed=0;
 rotSpeed=0;
}

Continent Continent::split(mt19937_64* mtGen)
{
 int beginID=randint((*mtGen)(), 0, shape.points.size()-1);
 int prevID=(beginID==0 ? shape.points.size()-1 : beginID-1);
 int nextID=(beginID+1)%shape.points.size();
 vector<Coord> cutLine;
 double curDir=(angleCoord(shape.points[beginID], shape.points[prevID])+angleCoord(shape.points[beginID], shape.points[nextID]))/2;
 double curRotSpeed=0;

 Continent newCont;

 if (angleCoord(shape.points[beginID], shape.points[prevID])<angleCoord(shape.points[beginID], shape.points[nextID])) curDir+=M_PI;

 cutLine.push_back(shape.points[beginID]+CoordVector(curDir+randdouble((*mtGen)())*(M_PI/2)-M_PI/4, CONTINENTS_CUT_SEG_LEN));

 while (true)
 {
  curRotSpeed=0.9*curRotSpeed+(randdouble((*mtGen)())*M_PI/100-M_PI/200);
  curDir+=curRotSpeed;
  //cutLine.push_back(cutLine.back()+CoordVector(curDir+randdouble((*mtGen)())*(M_PI/2)-M_PI/4, CONTINENTS_CUT_SEG_LEN));
  cutLine.push_back(cutLine.back()+CoordVector(curDir, CONTINENTS_CUT_SEG_LEN)+CoordVector(curDir+M_PI/2, (randdouble((*mtGen)())*WORLD_SIZE/50-WORLD_SIZE/100)));

  if (!shape.isInside(cutLine.back()))
  {
   cutLine.pop_back();
   break;
  }
  bool exit=false;
  for (unsigned int i=0; i<shape.points.size(); i++)
  {
   if (distCoord(cutLine.back(), shape.points[i])<CONTINENTS_CUT_SEG_LEN) exit=true;
  }
  if (exit) break;
 }

 newCont.shape=shape.cut(cutLine);
 return newCont;
}

void Continent::move()
{
 movement.angle+=mvtRotSpeed;
 shape.origin+=movement;
 shape.orientation+=rotSpeed;

 if (shape.origin.x<0) shape.origin.x+=WORLD_SIZE;
 if (shape.origin.x>=WORLD_SIZE) shape.origin.x-=WORLD_SIZE;
 if (shape.origin.y<0) shape.origin.y+=WORLD_SIZE;
 if (shape.origin.y>=WORLD_SIZE) shape.origin.y-=WORLD_SIZE;
}

Pangea::Pangea(mt19937_64* mtGen)
{
 generate(mtGen);
}

void Pangea::generate(mt19937_64* mtGen)
{
 int currentDist=PANGEA_SEG0_DIST;
 seg[0]=currentDist;

 for (int i=1; i<PANGEA_NB_SEGS; i++)
 {
  int joinTotalDelta=PANGEA_JOIN_DELTA*(PANGEA_NB_SEGS-i);
  int minimum=max(PANGEA_MIN_SEG_DIST, PANGEA_SEG0_DIST-joinTotalDelta);
  int maximum=min(PANGEA_MAX_SEG_DIST, PANGEA_SEG0_DIST+joinTotalDelta);
  int minDelta=max(minimum-currentDist, -PANGEA_MAX_DELTA);
  int maxDelta=min(maximum-currentDist, PANGEA_MAX_DELTA);
  currentDist+=randint((*mtGen)(), minDelta, maxDelta);
  seg[i]=currentDist;
 }
}

/*void Pangea::draw()
{
 for (int i=0; i<PANGEA_NB_SEGS; i++)
 {
  int d1=seg[i]/10;
  int d2=seg[(i+1)%PANGEA_NB_SEGS]/10;
  double angle1=i*2*M_PI/PANGEA_NB_SEGS;
  double angle2=((i+1)%PANGEA_NB_SEGS)*2*M_PI/PANGEA_NB_SEGS;
  thickLineRGBA (surface, cos(angle1)*d1+300, sin(angle1)*d1+300, cos(angle2)*d2+300, sin(angle2)*d2+300, 2, 255, 255, 255, SDL_ALPHA_OPAQUE);
 }
}*/

Continent Pangea::exportCont()
{
 Continent output;

 for (int i=0; i<PANGEA_NB_SEGS; i++)
 {
  output.shape.points.push_back(Coord(CoordVector(i*2*M_PI/PANGEA_NB_SEGS, seg[i])));
 }

 return output;
}

Pangea::operator Continent()
{
 return exportCont();
}



ContRelMod::ContRelMod()
{
 type=CONTRELMOD_NONE;
}

ContRelMod::ContRelMod(Coord pos, double pow, int t)
{
 position=pos;
 power=pow;
 type=t;
}

void handlePotColision(Continent* a, Continent* b)
{
 // due to cyclic properties, adapt origin
 while ((a->shape.origin.x - b->shape.origin.x) > WORLD_SIZE/2) a->shape.origin.x-=WORLD_SIZE;
 while ((a->shape.origin.x - b->shape.origin.x) < -WORLD_SIZE/2) a->shape.origin.x+=WORLD_SIZE;
 while ((a->shape.origin.y - b->shape.origin.y) > WORLD_SIZE/2) a->shape.origin.y-=WORLD_SIZE;
 while ((a->shape.origin.y - b->shape.origin.y) < -WORLD_SIZE/2) a->shape.origin.y+=WORLD_SIZE;

 if (overlapShapeFast(a->shape, b->shape))
 {
  // add ContRelMod objects
  for (unsigned int i=0; i<a->shape.points.size(); i++)
  {
   if (b->shape.isInside(a->shape.exportPoint(i)))
   {
    b->newContRelMods.push_back(ContRelMod(a->shape.exportPoint(i), distCoord(a->movement, b->movement)*CRM_COL_COEF/WORLD_SIZE, CONTRELMOD_COLISION));
   }
  }
  for (unsigned int i=0; i<b->shape.points.size(); i++)
  {
   if (a->shape.isInside(b->shape.exportPoint(i)))
   {
    a->newContRelMods.push_back(ContRelMod(b->shape.exportPoint(i), distCoord(a->movement, b->movement)*CRM_COL_COEF/WORLD_SIZE, CONTRELMOD_COLISION));
   }
  }

  // modify direction
  double aProp=a->shape.calcArea() / (a->shape.calcArea()+b->shape.calcArea());
  double bProp=1-aProp;

  CoordVector meanMvt=aProp*a->movement + bProp*b->movement;
  double meanRot=aProp*a->rotSpeed + bProp*b->rotSpeed;
  double meanMvtRot=aProp*a->mvtRotSpeed + bProp*b->mvtRotSpeed;

  double coefKeep=0.95;
  double coefTake=1-coefKeep;
  a->movement=coefKeep*a->movement+coefTake*meanMvt;
  b->movement=coefKeep*b->movement+coefTake*meanMvt;
  a->rotSpeed=coefKeep*a->rotSpeed+coefTake*meanRot;
  b->rotSpeed=coefKeep*b->rotSpeed+coefTake*meanRot;
  a->mvtRotSpeed=coefKeep*a->mvtRotSpeed+coefTake*meanMvtRot;
  b->mvtRotSpeed=coefKeep*b->mvtRotSpeed+coefTake*meanMvtRot;
 }

 if (a->shape.origin.x<0) a->shape.origin.x+=WORLD_SIZE;
 if (a->shape.origin.x>=WORLD_SIZE) a->shape.origin.x-=WORLD_SIZE;
 if (a->shape.origin.y<0) a->shape.origin.y+=WORLD_SIZE;
 if (a->shape.origin.y>=WORLD_SIZE) a->shape.origin.y-=WORLD_SIZE;
}
