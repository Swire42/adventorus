#include <iostream>
#include <vector>

using namespace std;

uint16_t location8(uint8_t x, uint8_t y)
{
 return (uint16_t)(x)+((uint16_t)(y)<<8);
}

uint32_t location16(uint16_t x, uint16_t y)
{
 return (uint32_t)(x)+((uint32_t)(y)<<16);
}

template<typename Type> void showBinDec(Type n)
{
 for (int iByte=sizeof(Type)-1; iByte>=0; iByte--)
 {
  for (int iBit=7; iBit>=0; iBit--)
  {
   cout << bool(n&(1<<(iByte*8+iBit)));
  }
  cout << " ";
 }

 cout << "(" << n << ")\n";
}

struct Pos
{
 uint16_t regionX, regionY;
 uint8_t chunkX, chunkY;

 Pos(uint8_t cX, uint8_t cY, uint16_t rX, uint16_t rY)
 {
  chunkX=cX;
  chunkY=cY;
  regionX=rX;
  regionY=rY;
 }

 void show()
 {
  cout << unsigned(chunkX) << ";" << unsigned(chunkY) << " in "
   << regionX << ";" << regionY << endl;
 }
};

#define METHOD_INVERT 0
#define METHOD_ISTHISHASH 1

uint32_t generateKey(int method, uint16_t a, uint32_t b, bool talk=false)
{
 uint32_t key=0;
 switch (method)
 {
  case METHOD_INVERT:
   key=b;

   for (int i=0; i<16; i++)
   {
    if (a&(1<<i))
    {
     // need to invert
     key^=0b11<<(i*2);
    }
   }
   break;
  case METHOD_ISTHISHASH:
   key=b;
   for (int i=0; i<32; i++)
   {
    if ( ( (!(i&1)) && (a&(1<<(i/2))) ) // even
      || ( (i&1) && (a&(1<<(15-i/2))) ) )
    {
     key^=1<<i;
    }
   }
   break;
 }

 if (talk)
 {
  cout << "generated : ";
  showBinDec(key);
  cout << "from : ";
  showBinDec(a);
  cout << "and : ";
  showBinDec(b);
 }

 return key;
}

vector<Pos> invertHash(uint32_t key)
{
 vector<Pos> positions;
 for (uint8_t chunkX=0; chunkX<64; chunkX++)
 {
  for (uint8_t chunkY=0; chunkY<64; chunkY++)
  {
   uint32_t tempKey=key;
   uint16_t a=location8(chunkX, chunkY);
   for (int i=0; i<32; i++)
   {
    if ( ( (!(i&1)) && (a&(1<<(i/2))) ) // even
      || ( (i&1) && (a&(1<<(15-i/2))) ) )
    {
     tempKey^=1<<i;
    }
   }

   if ((((tempKey<<16)>>16)<10000) && ((tempKey>>16)<10000))
   {
    //cout << unsigned(chunkX) << ";" << unsigned(chunkY) << " in " << ((tempKey<<16)>>16) << ";" << (tempKey>>16) << endl;
    positions.push_back(Pos(chunkX, chunkY, (tempKey<<16)>>16, tempKey>>16));
   }
  }
 }
 return positions;
}

#define maxRegion 100

int main()
{
 uint32_t what2count[10]={
  263882213, 2804616432, 41854549, 220892325, 628043441,
  2239432372, 2788820080, 205399077, 71279669, 802139105};
 int counter[10]={
  0, 0, 0, 0, 0,
  0, 0, 0, 0, 0};

 /*for (uint16_t regionX=0; regionX<maxRegion; regionX++)
 {
  cout << regionX*100.0/maxRegion << "%\n";
  for (uint16_t regionY=0; regionY<maxRegion; regionY++)
  {
   for (uint16_t chunkX=0; chunkX<64; chunkX++)
   {
    for (uint16_t chunkY=0; chunkY<64; chunkY++)
    {
     for (int i=0; i<10; i++)
     {
      if(generateKey(METHOD_ISTHISHASH, location8(chunkX, chunkY), location16(regionX, regionY))==what2count[i]) counter[i]++;
     }
    }
   }
  }
 }

 for (int i=0; i<10; i++)
 {
  cout << "Found " << what2count[i] << " " << counter[i] << " times.\n";
 }*/

 vector<Pos> positions;
 positions=invertHash(what2count[0]);
 cout << "There are " << positions.size() << " positions available\n";

 for (unsigned int i=0; i<positions.size(); i++)
 {
  cout << "next is : " << generateKey(METHOD_ISTHISHASH, location8(positions[i].chunkX+1, positions[i].chunkY), location16(positions[i].regionX, positions[i].regionY)) << endl;
 }

 return 0;
}
