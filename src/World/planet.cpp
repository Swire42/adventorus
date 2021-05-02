#include "planet.hpp"
#include <random>
#include <algorithm>
#include "MyMath/v2/MyMath.hpp"
#include "My2DFuncs/v1/MyGaussian.hpp"
#include "My2DFuncs/v1/MySimplex.hpp"

using namespace std;

// Conts deriv.
#define DERIV_DUR (150)

// heightMap heights
#define OCEAN_FLOOR (-2000)
#define GROUND_FLOOR (20)
#define SUPERPOSED_FLOOR (4000)

// CRM=ContRelMod
#define CRM_COL_RAD (WORLD_SIZE/100)

World::World(uint64_t _key)
{
 key=_key;
}

void World::generate()
{
 /*generateConts();
 doContinentalDrift();
 initRelief();
 applyContRelMods();*/
 heightMap=vector<vector<int16_t> >(WORLD_SIZE, vector<int16_t>(WORLD_SIZE, 0));
 //addNoises();
 setSunMap();
 /*windErosion(5);
 setSunMap();
 setWindMap();
 setRainMap();
 waterErosion();
 waterErosion();
 waterErosion();*/
}

/*void World::generateConts()
{
 mt19937_64 mtGen;
 mtGen.seed(key);

 vector<Continent> tempConts;

 tempConts.push_back(Pangea(&mtGen));

 while (!tempConts.empty())
 {
  // determine if we cut
  double n1=tempConts.back().shape.calcArea()/(M_PI*PANGEA_MIN_SEG_DIST*PANGEA_MIN_SEG_DIST);
   //(double(int(tempConts.back().shape.points.size())-250)/250.0);
  double n2=randdouble(mtGen());

  if ((n1>n2) || (conts.size()+tempConts.size()<3))
  {
   tempConts.push_back(tempConts.back().split(&mtGen));
  }
  else
  {
   conts.push_back(tempConts.back());
   tempConts.pop_back();
  }
 }

 for (unsigned int i=0; i<conts.size(); i++)
 {
  conts[i].shape.setOriginTo(conts[i].shape.getCentroid());
 }
}

void World::doContinentalDrift()
{
 mt19937_64 mtGen;
 mtGen.seed(key);

 for (unsigned int i=0; i<conts.size(); i++)
 {
  conts[i].movement=conts[i].shape.origin/100/(1+randdouble(mtGen()));
  conts[i].mvtRotSpeed=randdouble(mtGen())*M_PI/200-M_PI/400;
  conts[i].rotSpeed=randdouble(mtGen())*M_PI/1000-M_PI/2000;
 }

 cout << "---------- simulating conts deriv.\r";
 for (unsigned int t=0; t<DERIV_DUR; t++)
 {
  if ((t+1)%(DERIV_DUR/10)==0) cout << "#" << flush;
  for (unsigned int i=0; i<conts.size(); i++)
  {
   conts[i].move();
  }
  handleContsColisions();
 }
 cout << endl;
}

void World::handleContsColisions()
{
 /// to speed up (radically) the process, we export all the shapes to global referencial :

 // save local shapes && export to global ref.
 vector<Shape> localShapes;
 for (unsigned int i=0; i<conts.size(); i++)
 {
  localShapes.push_back(conts[i].shape);
  conts[i].shape=conts[i].shape.exportNoOrient();
 }

 for (unsigned int aID=0; aID<conts.size(); aID++)
 {
  for (unsigned int bID=aID+1; bID<conts.size(); bID++)
  {
   handlePotColision(&conts[aID], &conts[bID]);
  }
 }

 // reload local ref.
 for (unsigned int i=0; i<conts.size(); i++)
 {
  conts[i].shape=localShapes[i];
  for (unsigned int j=0; j<conts[i].newContRelMods.size(); j++)
  {
   Coord c=conts[i].newContRelMods[j].position-conts[i].shape.origin;
   while (c.x>WORLD_SIZE/2) c.x-=WORLD_SIZE;
   while (c.x<-WORLD_SIZE/2) c.x+=WORLD_SIZE;
   while (c.y>WORLD_SIZE/2) c.y-=WORLD_SIZE;
   while (c.y<-WORLD_SIZE/2) c.y+=WORLD_SIZE;
   CoordVector v(c);
   v.angle-=conts[i].shape.orientation;
   conts[i].contRelMods.push_back(ContRelMod(v, conts[i].newContRelMods[j].power, conts[i].newContRelMods[j].type));
  }
  conts[i].newContRelMods.clear();
 }
}

void World::initRelief()
{
 for (unsigned int i=0; i<conts.size(); i++)
 {
  for (unsigned int j=0; j<conts[i].contRelMods.size(); j++)
  {
   CoordVector v=conts[i].contRelMods[j].position;
   v.angle+=conts[i].shape.orientation;
   conts[i].contRelMods[j].position=v+conts[i].shape.origin;
  }
  conts[i].shape=conts[i].shape.exportShape();
 }
 heightMap.clear();
 vector<int16_t> buf;
 buf.resize(WORLD_SIZE, OCEAN_FLOOR);
 heightMap.resize(WORLD_SIZE, buf);
 cout << "---------- initializing relief\r";
 for (unsigned int x=0; x<WORLD_SIZE; x++)
 {
  if ((x+1)%(WORLD_SIZE/10)==0) cout << "#" << flush ;
  //heightMap.push_back(vector<int16_t>());
  //heightMap[x].resize(WORLD_SIZE, -4000);

  // establish changes locations
  for (unsigned int i=0; i<conts.size(); i++)
  {
   vector<int> changes;
   for (unsigned int p=0; p<conts[i].shape.points.size(); p++)
   {
    Coord coordA=conts[i].shape.points[p];
    Coord coordB=conts[i].shape.points[(p+1)%conts[i].shape.points.size()];

    if ((coordA.x<x) && (coordB.x<x))
    {
     coordA.x+=WORLD_SIZE;
     coordB.x+=WORLD_SIZE;
    }
    if ((coordA.x>x) && (coordB.x>x))
    {
     coordA.x-=WORLD_SIZE;
     coordB.x-=WORLD_SIZE;
    }
    if ((coordA.x<=x) != (coordB.x<=x))
    {
     AffineFunc d(coordA, coordB);
     changes.push_back(int(d.image(x)));
    }
   }
   sort(changes.begin(), changes.end());

   for (unsigned int c=0; c<changes.size(); c+=2)
   {
    for (int _y=changes[c]; _y<changes[c+1]; _y++)
    {
     int y=_y;
     if (y<0) y+=WORLD_SIZE;
     else if (y>=WORLD_SIZE) y-=WORLD_SIZE;
     if (heightMap[x][y]==OCEAN_FLOOR) heightMap[x][y]=GROUND_FLOOR;
     else if (heightMap[x][y]==GROUND_FLOOR) heightMap[x][y]=SUPERPOSED_FLOOR;
    }
   }
  }
 }
 cout << endl;
}

void World::applyContRelMods()
{
 int total=0;
 int counter=0;
 cout << "---------- applying modifiers\r" << flush;
 for (unsigned int i=0; i<conts.size(); i++)
 {
  total+=conts[i].contRelMods.size();
 }
 for (unsigned int i=0; i<conts.size(); i++)
 {
  for (unsigned int j=0; j<conts[i].contRelMods.size(); j++)
  {
   counter++;
   if ((counter%(total/10))==0) cout << "#" << flush;

   if (conts[i].contRelMods[j].type==CONTRELMOD_COLISION)
   {
    for (int dX=-CRM_COL_RAD; dX<=CRM_COL_RAD; dX++)
    {
     for (int dY=-CRM_COL_RAD; dY<=CRM_COL_RAD; dY++)
     {
      if ((dX*dX+dY*dY)<=(CRM_COL_RAD*CRM_COL_RAD))
      {
       int x=floor(conts[i].contRelMods[j].position.x+dX);
       int y=floor(conts[i].contRelMods[j].position.y+dY);
       while (x<0) x+=WORLD_SIZE;
       while (y<0) y+=WORLD_SIZE;
       while (x>=WORLD_SIZE) x-=WORLD_SIZE;
       while (y>=WORLD_SIZE) y-=WORLD_SIZE;
       heightMap[x][y]+=conts[i].contRelMods[j].power;
      }
     }
    }
   }
  }
 }
 cout << endl;
}

void World::windErosion(int radius)
{
 cout << "?????????? applying wind erosion\r" << flush;
 heightMap=gaussianBlur(heightMap, radius, true);
 cout << "##########\n";
}
*/

#define disp(x) std::cout << #x << ":" << x << "\n"
void World::addNoises()
{
 //const double c=WORLD_SIZE*sqrt(3.0);
 /*cout << "---------- adding noises\r" << flush;

 uint64_t tempKey=key;

 for (unsigned int i=1; true; i*=2)
 {
  double size=WORLD_SIZE/4.0/i;
  double coef=-100*sqrt((250-size)*(250-size)+3120*3120)+100*3120+1000;
  if (coef<0.5) break;
  cout << "size=" << size << " ";
  addNoise(i, coef, tempKey++);
 }*/
 /*cout << "##" << flush;
 addNoise(WORLD_SIZE/5.0, 100, tempKey++);
 cout << "#" << flush;
 addNoise(WORLD_SIZE/14.0, 250, tempKey++);
 cout << "#" << flush;
 addNoise(WORLD_SIZE/14.0, 250, tempKey++);
 cout << "#" << flush;
 addNoise(WORLD_SIZE/71.0, 125, tempKey++);
 cout << "##" << flush;
 addNoise(WORLD_SIZE/71.0, 125, tempKey++);
 cout << "#" << flush;
 addNoise(WORLD_SIZE/142.0, 25, tempKey++);
 cout << "#" << flush;
 addNoise(WORLD_SIZE/194.0, 10, tempKey++);
 cout << "#" << endl;*/

 cout << "---------- adding noises\r" << flush;

 uint64_t tempKey=key;
 vector<CyclicNoise> baseNoise;
 vector<CyclicNoise> coefNoise1;
 vector<CyclicNoise> coefNoise2;

 for (unsigned int i=1; true; i*=2)
 {
  double size=WORLD_SIZE/4.0/i;
  double coef=-100*sqrt((250-size)*(250-size)+3120*3120)+100*3120+1000;
  if (coef<0.5) break;
  baseNoise.push_back(CyclicNoise(tempKey++, i, i));
 }
 for (unsigned int i=2; i<=8; i*=2)
 {
  coefNoise1.push_back(CyclicNoise(tempKey++, i, i));
  coefNoise2.push_back(CyclicNoise(tempKey++, i, i));
 }

 for (unsigned int x=0; x<WORLD_SIZE; x++)
 {
  for (unsigned int y=0; y<WORLD_SIZE; y++)
  {
   double val=0;
   for (unsigned int i=0; i<baseNoise.size(); i++)
   {
    int subdiv=(1<<i);
    double size=WORLD_SIZE/4.0/subdiv;
    double coef=-100*sqrt((250-size)*(250-size)+3120*3120)+100*3120+1000;
    double pX=double(x)*subdiv/WORLD_SIZE;
    double pY=double(y)*subdiv/WORLD_SIZE;
    val+=coef*baseNoise[i].eval(pX, pY);
   }
   double coefval1=0;
   double coefval2=0;
   for (unsigned int i=0; i<coefNoise1.size(); i++)
   {
    int subdiv=1<<(i+1);
    double size=WORLD_SIZE/4.0/subdiv;
    double coef=size/250;
    double pX=double(x)*subdiv/WORLD_SIZE;
    double pY=double(y)*subdiv/WORLD_SIZE;
    double noise1=coefNoise1[i].eval(pX, pY);
    double noise2=coefNoise2[i].eval(pX, pY);
    coefval1+=coef*(noise1*0.4+0.6);
    coefval2+=coef*(noise2*0.4+0.6);
   }
   heightMap[x][y]=round(coefval1*coefval1*coefval2*coefval2*val*2);
   //heightMap[x][y]=round(coefval1*1000);
  }
 }

}

void World::addNoise(int size, double coef, uint64_t aKey)
{
 //mt19937_64 mtGen(aKey);
 CyclicNoise noise(aKey, size, size);

 double max=0;
 //double dX=randdouble(mtGen())*sqrt(3);
 //double dY=randdouble(mtGen())*sqrt(3);
 for (unsigned int x=0; x<WORLD_SIZE; x++)
 {
  for (unsigned int y=0; y<WORLD_SIZE; y++)
  {
   double pX=double(x)*size/WORLD_SIZE;
   double pY=double(y)*size/WORLD_SIZE;
   //if (pX>size) pX-=size;
   //if (pY>size) pY-=size;
   double val=coef*noise.eval(pX, pY);
   if (max<val) max=val;
   heightMap[x][y]+=round(val);
  }
 }
 cout << "max=" << max << " (coef=" << coef << ")\n";
}

void World::setSunMap(uint8_t day, uint8_t time, uint8_t sec)
{
 sunMap=vector<vector<uint8_t> >(WORLD_SIZE, vector<uint8_t>(WORLD_SIZE, 0));
 for (unsigned int x=0; x<WORLD_SIZE; x++)
 {
  for (unsigned int y=0; y<WORLD_SIZE; y++)
  {
   sunMap[x][y]=getSunRay(x, y, day, time, sec).lenght()*0xFF;
   //disp(sunMap[x][y]);
   //cout << getSunRay(x, y, 0, 0, 0).lenght() << endl;
  }
 }
}

void World::setWindMap()
{
}

void World::setRainMap()
{
 rainMap.clear();
 vector<uint8_t> buf;
 buf.resize(WORLD_SIZE, 255);
 rainMap.resize(WORLD_SIZE, buf);
}

struct Location
{
 uint16_t x, y;
 //int16_t height;
 //uint8_t rain;

 Location() {}
 Location(uint16_t _x, uint16_t _y)//, int16_t _h, uint8_t _r)
 {
  x=_x;
  y=_y;
  //height=_h;
  //rain=_r;
 }
};

/*bool isLower(Location a, Location b)
{
 return a.height < b.height;
}*/

#define UNSET (-32000) // approx int16 negative limit

extern void (*showHM)(vector<vector<int16_t> >);

void World::waterErosion()
{
 cout << "---------- applying water erosion\r" << flush;
 vector< vector<int16_t> > oldHMap;
 vector< vector<uint16_t> > water;
 vector< vector<uint16_t> > sedim;
 oldHMap=heightMap;
 for (unsigned int x=0; x<WORLD_SIZE; x++)
 {
  water.push_back(vector<uint16_t>());
  sedim.push_back(vector<uint16_t>());
  for (unsigned int y=0; y<WORLD_SIZE; y++)
  {
   water[x].push_back(rainMap[x][y]);
   sedim[x].push_back(0);
  }
 }
 vector<Location> stack;

 for (unsigned int x=0; x<WORLD_SIZE; x++)
 {
  for (unsigned int y=0; y<WORLD_SIZE; y++)
  {
   heightMap[x][y]=UNSET;
  }
 }

 int nbProcessed=0;
 int xToCheck=0;
 int yToCheck=0;

 while (nbProcessed<WORLD_SIZE*WORLD_SIZE)
 {
  if (stack.empty())
  {
   while (heightMap[xToCheck][yToCheck]!=UNSET)
   {// find a not processed cell
    xToCheck++;
    if (xToCheck==WORLD_SIZE)
    {
     xToCheck=0;
     yToCheck++;
    }
   }
   stack.push_back(Location(xToCheck, yToCheck));
  }

  // check if upper cells where processed.
  bool pushed=false;
  for (int dx=-1; dx<=1; dx++)
  {
   for (int dy=-1; dy<=1; dy++)
   {
    int x=stack.back().x+dx;
    if (x==-1) x=WORLD_SIZE-1;
    if (x==WORLD_SIZE) x=0;
    int y=stack.back().y+dy;
    if (y==-1) y=WORLD_SIZE-1;
    if (y==WORLD_SIZE) y=0;

    if ((oldHMap[x][y]>oldHMap[stack.back().x][stack.back().y]) && (heightMap[x][y]==UNSET))
    {
     pushed=true;
     stack.push_back(Location(x, y));
     break;
    }
   }
   if (pushed) break;
  }

  if (!pushed)
  {// curent cell can be processed
   int cx=stack.back().x;
   int cy=stack.back().y;
   int16_t newH=oldHMap[cx][cy];
#warning todo // set quantities of water for lower cells and newH
#warning todo // current model is really bad
   int totalDH=0;
   for (int dx=-1; dx<=1; dx++)
   {
    for (int dy=-1; dy<=1; dy++)
    {
     int x=cx+dx;
     if (x==-1) x=WORLD_SIZE-1;
     if (x==WORLD_SIZE) x=0;
     int y=cy+dy;
     if (y==-1) y=WORLD_SIZE-1;
     if (y==WORLD_SIZE) y=0;

     if (oldHMap[x][y]<oldHMap[cx][cy])
     {
      totalDH+=oldHMap[cx][cy]-oldHMap[x][y];
     }
    }
   }
   double drop;
   if (totalDH>=1000) drop=0;
   else //if (totalDH!=0)
   {
    drop=0.5-totalDH/2000.0;
   }
   int dropQt=round(sedim[cx][cy]*drop);
   sedim[cx][cy]-=dropQt;
   newH+=dropQt;

   int pickQt=totalDH*(water[cx][cy]/2550.0); // 2550=max water
   sedim[cx][cy]+=pickQt;
   newH-=pickQt;
   if (sedim[cx][cy]>totalDH) sedim[cx][cy]=totalDH;

   int miniH=oldHMap[cx][cy];
   for (int dx=-1; dx<=1; dx++)
   {
    for (int dy=-1; dy<=1; dy++)
    {
     int x=cx+dx;
     if (x==-1) x=WORLD_SIZE-1;
     if (x==WORLD_SIZE) x=0;
     int y=cy+dy;
     if (y==-1) y=WORLD_SIZE-1;
     if (y==WORLD_SIZE) y=0;

     if (oldHMap[x][y]<miniH) miniH=oldHMap[x][y];

     if (oldHMap[x][y]<oldHMap[cx][cy])
     {
      int dh=oldHMap[cx][cy]-oldHMap[x][y];
      water[x][y]+=water[cx][cy]*dh/totalDH*0.9;
      sedim[x][y]+=sedim[cx][cy]*dh/totalDH*0.9;
     }
    }
   }

   if (newH<miniH) newH=miniH;
   heightMap[cx][cy]=newH;
   nbProcessed++;
   if (nbProcessed%(WORLD_SIZE*WORLD_SIZE/100)==0)
   {
    cout << int(heightMap[cx][cy])-oldHMap[cx][cy] << "|" << sedim[cx][cy] << endl;;
    showHM(heightMap);
   }
   if (nbProcessed%(WORLD_SIZE*WORLD_SIZE/10)==0) cout << "#" << flush;
   stack.pop_back();
  }
 }
 cout << "\n";
}
