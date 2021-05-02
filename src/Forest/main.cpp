#include "Coords/v2/Coords.hpp"
#include "MyMath/v2/MyMath.hpp"
#include "EasySDL/v6/EasySDL.hpp"
#include <vector>
#include <random>

using namespace std;

#define TREE_DIAMETER 4.0
#define DENSITY 1.2
#define MAX_ATTEMPTS 20

#define MIN_DIST (TREE_DIAMETER/DENSITY)
#define MAX_DIST (MIN_DIST*2)

#define CHUNK_SIZE 16

#define SCALE 32

struct Chunk
{
 vector<Coord> activeTrees;
 vector<Coord> trees;

 Chunk();
 Chunk(uint64_t worldKey, uint64_t chunkKey);

 void generate(uint64_t worldKey, uint64_t chunkKey);
 void addTree(int src);
};

Chunk::Chunk() {}

Chunk::Chunk(uint64_t worldKey, uint64_t chunkKey)
{
 generate(worldKey, chunkKey);
}

void Chunk::generate(uint64_t worldKey, uint64_t chunkKey)
{
 mt19937_64 mtGen;
 mtGen.seed(worldKey^chunkKey);

 activeTrees.clear();
 trees.clear();

 trees.push_back(Coord(randdouble(mtGen())*CHUNK_SIZE, randdouble(mtGen())*CHUNK_SIZE));
 activeTrees.push_back(trees[0]);
 cout << trees[0].x << ";" << trees[0].y << endl;
 cout << MAX_DIST << endl;

 while (!activeTrees.empty())
 {
  // try to add a tree
  bool stayActive=false;
  unsigned int currentTree=mtGen()%activeTrees.size();

  for (unsigned int i=0; i<MAX_ATTEMPTS; i++)
  {
   double dist=randdouble(mtGen())*(MAX_DIST-MIN_DIST)+MIN_DIST;
   double angle=randdouble(mtGen())*M_PI*2;
   cout << dist << "&" << angle << endl;
   Coord pos=activeTrees[currentTree]+CoordVector(angle, dist);
   if ((pos.x<0) || (pos.x>=CHUNK_SIZE) || (pos.y<0) || (pos.y>=CHUNK_SIZE))
   {
    continue;
   }

   bool freePos=true;
   double mini=MIN_DIST*MIN_DIST;
   for (unsigned int j=0; j<trees.size(); j++)
   {
    double x=trees[j].x-pos.x;
    double y=trees[j].y-pos.y;
    cout << (x*x+y*y) << endl;
    if ((x*x+y*y)<mini)
    {
     freePos=false;
     break;
    }
   }
   if (freePos)
   {
    trees.push_back(pos);
    activeTrees.push_back(pos);
    stayActive=true;
    break;
   }
  }
  if (!stayActive)
  {
   activeTrees.erase(activeTrees.begin()+currentTree);
  }
 }
}

int main()
{
 uint64_t key;
 cin >> key;

 Chunk ch1(key, 1);

 initEasySDL(512, 512);

 while (!quit)
 {
  window::events();

  for (unsigned int i=0; i<ch1.trees.size(); i++)
  {
   filledCircleRGBA(surface, ch1.trees[i].x*SCALE, ch1.trees[i].y*SCALE, TREE_DIAMETER*SCALE/2, 0, 128, 0, 128);
  }

  window::refresh();
  wait(30);
 }

 return 0;
}
