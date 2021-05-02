#include "leaves.hpp"
#include <random>
#include "MyMath/v2/MyMath.hpp"

Leave::Leave(uint64_t seed)
{
 mt19937_64 mtGen;
 mtGen.seed(seed);

 const double xDeltaMin=0.4;
 const double xDeltaMax=0.6;
 double xDelta=randdouble(mtGen())*(xDeltaMax-xDeltaMin)+xDeltaMin;

 double xMeanMin=xDelta/2+0.1;
 double xMeanMax=1-xDelta/2-0.1;
 double xMean=randdouble(mtGen())*(xMeanMax-xMeanMin)+xMeanMin;

 A.x=xMean-xDelta/2;
 B.x=xMean+xDelta/2;

 const double yMeanMin=0.2;
 const double yMeanMax=0.4;
 double yMean=randdouble(mtGen())*(yMeanMax-yMeanMin)+yMeanMin;

 const double yPropMin=-0.2;
 const double yPropMax=0.2;
 double yProp=randdouble(mtGen())*(yPropMax-yPropMin)+yPropMin;

 A.y=yMean*(1-yProp);
 B.y=yMean*(1+yProp);

 const double greenMin=0.33;
 const double greenMax=0.75;
 double green=randdouble(mtGen())*(greenMax-greenMin)+greenMin;
 r=0;
 g=green*255;
 b=0;
}
