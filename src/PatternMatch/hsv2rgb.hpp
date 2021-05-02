#pragma once

struct Rgb
{
 double r,g,b;

 Rgb() {}
 Rgb(double _r, double _g, double _b) : r(_r), g(_g), b(_g) {}
};

struct Hsv
{
 double h,s,v;

 Hsv() {}
 Hsv(double _h, double _s, double _v) : h(_h), s(_s), v(_v) {}
};

Rgb hsv2rgb(Hsv in);
