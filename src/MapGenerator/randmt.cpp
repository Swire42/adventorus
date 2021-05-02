#include <iostream>
#include <random>

using namespace std;

int main()
{
 uint64_t key=42;
 mt19937_64 generator(key);

 cout << "begin\n";
 generator.discard(10000);
 cout << "end\n";

 for (int i=0; i<100; i++)
 {
  cout << generator() << endl;
 }

 return 0;
}
