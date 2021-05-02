#include <iostream>
#include <vector>
#include "map.hpp"

using namespace std;

uint64_t getKey(uint32_t seed, Pos position)
{
 uint64_t key=position.assembled();
 for (int i=0; i<64; i++)
 {
  if ( ( (!(i&1)) && (seed&(1<<(i/2))) ) // even
    || ( (i&1) && (seed&(1<<(31-i/2))) ) )
  {
   key^=1<<i;
  }
 }
 return key;
}

int main()
{
 int seed=42;
 cout << getKey(seed, Pos(11, 54, 1855, 57)) << endl;
 return 0;
}
