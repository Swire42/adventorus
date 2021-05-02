#include "maillage.hpp"
#include "MyMath/v2/MyMath.hpp"
#include "Geometry/v1/vector.hpp"

PDPoint::PDPoint(Point2D posi, double rad) {pos=posi; radius=rad;}

bool distOK(PDPoint a, PDPoint b)
{
 return (a.radius+b.radius)*(a.radius+b.radius)<sqDist(a.pos, b.pos);
}

GridPos::GridPos() {x=0; y=0; attempts=0;}
GridPos::GridPos(int _x, int _y) {x=_x; y=_y; attempts=0;}

MaillageChunk::MaillageChunk(uint64_t wS, ChunkPos p, double minR, double maxR)
{
 wSeed=wS;
 pos=p;

 minRad=minR;
 maxRad=maxR;
 double maxCellSize=minRad*sqrt(2);
 double minGridSize=1/maxCellSize;
 gridSize=ceil(minGridSize);
 cellSize=1.0/gridSize;
}

void MaillageChunk::init()
{
 mtGen.seed(genChunkSeed(wSeed, pos));
 grid=std::vector<std::vector<int> >(gridSize, std::vector<int>(gridSize, -1));

 available.clear();
 for (int x=0; x<gridSize; x++)
 for (int y=0; y<gridSize; y++)
 {
  available.push_back(GridPos(x, y));
 }
}

PDPoint MaillageChunk::next()
{
 if (available.size()==0) return PDPoint(Point2D(0.5, 0.5), 0.1); // dummy point

 int i=mtGen()%available.size();
 Point2D pos;
 pos.x=available[i].x*cellSize+randdouble(mtGen())*cellSize;
 pos.y=available[i].y*cellSize+randdouble(mtGen())*cellSize;

 available[i].attempts++;
 if ((grid[available[i].x][available[i].x]!=-1) || (available[i].attempts>50)) available.erase(available.begin()+i);

 return PDPoint( pos, randdouble(mtGen())*(maxRad-minRad)+minRad );
}

bool MaillageChunk::isFree(PDPoint p)
{
 int gridX=floor(p.pos.x/cellSize);
 int gridY=floor(p.pos.y/cellSize);

 //int checkDist=ceil(maxRad*2/cellSize);
 int checkDist=ceil(p.radius/cellSize)+1;

 int xStart=gridX-checkDist;
 if (xStart<0) xStart=0;
 if (xStart>gridSize-1) xStart=gridSize-1;
 int xEnd=gridX+checkDist;
 if (xEnd<0) xEnd=0;
 if (xEnd>gridSize-1) xEnd=gridSize-1;
 int yStart=gridY-checkDist;
 if (yStart<0) yStart=0;
 if (yStart>gridSize-1) yStart=gridSize-1;
 int yEnd=gridY+checkDist;
 if (yEnd<0) yEnd=0;
 if (yEnd>gridSize-1) yEnd=gridSize-1;

 for (int gX=xStart; gX<=xEnd; gX++)
 for (int gY=yStart; gY<=yEnd; gY++)
 {
  if ((grid[gX][gY]!=-1) && (!distOK(p, points[grid[gX][gY]])) ) return false;
 }

 return true;
}

void MaillageChunk::add(PDPoint p)
{
 int gridX=floor(p.pos.x/cellSize);
 int gridY=floor(p.pos.y/cellSize);

 int checkDist=floor((p.radius+minRad)/cellSize);

 int xStart=gridX-checkDist; if (xStart<0) xStart=0;
 int xEnd=gridX+checkDist+1; if (xEnd>gridSize-1) xEnd=gridSize-1;
 int yStart=gridY-checkDist; if (yStart<0) yStart=0;
 int yEnd=gridY+checkDist+1; if (yEnd>gridSize-1) yEnd=gridSize-1;

 for (int gX=xStart; gX<=xEnd; gX++)
 for (int gY=yStart; gY<=yEnd; gY++)
 {
  Point2D farCorner(gX*cellSize, gY*cellSize);
  double minSqDist=(p.radius+minRad)*(p.radius+minRad);
  if (
    (sqDist(p.pos, farCorner)<minSqDist) &&
    (sqDist(p.pos, farCorner+Vector2D_Co(cellSize, 0))<minSqDist) &&
    (sqDist(p.pos, farCorner+Vector2D_Co(0, cellSize))<minSqDist) &&
    (sqDist(p.pos, farCorner+Vector2D_Co(cellSize, cellSize))<minSqDist))
    grid[gX][gY]=points.size();
 }

 grid[gridX][gridY]=points.size();
 points.push_back(p);
}

void MaillageChunk::generate()
{
 std::vector<std::vector<MaillageChunk> > helpers;

 for (int x=0; x<3; x++)
 {
  helpers.push_back(vector<MaillageChunk>());
  for (int y=0; y<3; y++)
  {
   int dx=x-1;
   int dy=y-1;
   helpers[x].push_back(MaillageChunk(wSeed, pos.delta(dx, dy), minRad, maxRad));
   helpers[x][y].init();
  }
 }

 int failCount=0;

 while (!helpers[1][1].available.empty())
 {
  if (helpers[1][1].available.size()%100==0) cout << helpers[1][1].available.size() << endl;
  for (int sx=0; sx<3; sx++) // "s" for Source
  for (int sy=0; sy<3; sy++)
  {
   bool ok=true;
   int sdx=sx-1;
   int sdy=sy-1;
   PDPoint cur=helpers[sx][sy].next();
   cur.pos+=Vector2D_Co(sdx, sdy);

   for (int x=0; x<3; x++) // "s" for Source
   for (int y=0; y<3; y++)
   {
    int dx=x-1;
    int dy=y-1;
    PDPoint tmpCur=cur;
    tmpCur.pos-=Vector2D_Co(dx, dy);
    if (!helpers[x][y].isFree(tmpCur)) ok=false;
   }

   if (ok)
   {
    cur.pos-=Vector2D_Co(sdx, sdy);
    helpers[sx][sy].add(cur);
    failCount=0;
   }
   else failCount++;
  }
 }

 *this=helpers[1][1];
}
