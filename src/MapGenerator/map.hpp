#ifndef MAP_HPP
#define MAP_HPP

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

 uint64_t assembled()
 {
  int result=chunkX;
  result<<=16;
  result|=chunkY;
  result<<=16;
  result|=regionX;
  result<<=16;
  result|=regionY;
  return result;
 }
};

#endif // MAP_HPP
