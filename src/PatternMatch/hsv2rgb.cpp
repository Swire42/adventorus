#include "hsv2rgb.hpp"
#include <cmath>

Rgb hsv2rgb(Hsv in)
{
 double hh, p, q, t, ff;
 long i;
 Rgb out;

 if(in.s <= 0.0) // < is bogus, just shuts up warnings
 {
  out.r = in.v;
  out.g = in.v;
  out.b = in.v;
  return out;
 }

 hh = in.h;
 if(hh >= 2*M_PI) hh = 0.0;
 hh /= M_PI/3; // -> [0;6[
 i = (long)hh;
 ff = hh - i;
 p = in.v * (1.0 - in.s);
 q = in.v * (1.0 - (in.s * ff));
 t = in.v * (1.0 - (in.s * (1.0 - ff)));

 switch(i)
 {
 case 0:
  out.r = in.v;
  out.g = t;
  out.b = p;
  break;
 case 1:
  out.r = q;
  out.g = in.v;
  out.b = p;
  break;
 case 2:
  out.r = p;
  out.g = in.v;
  out.b = t;
  break;

 case 3:
  out.r = p;
  out.g = q;
  out.b = in.v;
  break;
 case 4:
  out.r = t;
  out.g = p;
  out.b = in.v;
  break;
 case 5:
 default:
  out.r = in.v;
  out.g = p;
  out.b = q;
  break;
 }
 return out;
}
