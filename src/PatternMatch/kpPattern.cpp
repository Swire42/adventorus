#include "EasySDL/v6/EasySDL.hpp"

#include "kpPattern.hpp"

using namespace std;

void KPPattern::input(int dir)
{
 lastEv=SDL_GetTicks();
 if (!recording)
 {
  recording=true;
  history.clear();
 }

 history.push_back(dir);
}

bool KPPattern::isFinished()
{
 if (!recording) return false;
 if (SDL_GetTicks()>lastEv+kpTimeout)
 {
  recording=false;
  return true;
 }
 else return false;
}

Order KPPattern::get()
{
 if (history.size()==1) return Order(history[0], typeSimple);

 else if (history.size()==2)
 {
  if (history[0]==history[1]) return Order(history[0], typeDouble);
  else if ((history[0]!=-1) && (history[1]!=-1) && (history[0]%4==history[1]%4)) return Order(history[0], typeReverse);
  else if ((history[0]!=-1) && (history[1]==-1)) return Order(history[0], typeReverse);
 }

 else if (history.size()==3)
 {
  if ((history[0]==history[1]) && (history[0]==history[2])) return Order(history[0], typeStrong);
 }

 return Order(-1, typeNone);
}
