#include "mousePattern.hpp"

using namespace std;

vector<array<double, nbSamples> > anglePatterns;
vector<array<GVector<2>, nbSamples> > vecPatterns;
vector<string> patternName;
vector<Order> patternOrder;

void MousePattern::clear()
{
 rawSampling.clear();
}

void MousePattern::reSample()
{
 for (unsigned int i=0; i<nbSamples; i++) constSampled[i]=GVector<2>({0,0});

 double tot=0;
 for (unsigned int i=0; i<rawSampling.size(); i++) tot+=rawSampling[i].len();

 int iSample=0;
 double pos=0;
 for (unsigned int i=0; i<rawSampling.size(); i++)
 {
  double sampleEnd=(iSample+1)*(tot/nbSamples);
  double endPos=pos+rawSampling[i].len();
  while (endPos>sampleEnd)
  {
   double prop=(sampleEnd-pos)/rawSampling[i].len();
   constSampled[iSample]+=rawSampling[i]*prop*(nbSamples/tot);
   iSample++;
   pos=sampleEnd;
   if (iSample==nbSamples)
   {
    // out of range
    //cout << "out by " << endPos-sampleEnd << endl;
    return;
   }
   sampleEnd=(iSample+1)*(tot/nbSamples);
  }

  constSampled[iSample]+=rawSampling[i]*(nbSamples/tot);

  pos=endPos;
 }
}

double MousePattern::percentMatch(array<GVector<2>, nbSamples> comp)
{
 double tot=0;
 const double maxDelta=M_PI/3;

 for (unsigned int i=0; i<nbSamples; i++)
 {
  if (abs(angle(constSampled[i], comp[i]))<maxDelta)
  {
   tot+=(1-abs(angle(constSampled[i], comp[i]))/maxDelta)/nbSamples;
  }
 }
 return tot;
}

void MousePattern::startRec()
{
 clear();
 recording=true;
}



Order MousePattern::validate()
{
 reSample();
 recording=false;

 int match=-1;
 double bestPercent=2/4.0;
 for (unsigned int i=0; i<vecPatterns.size(); i++)
 {
  double percent=percentMatch(vecPatterns[i]);
  if (percent>bestPercent)
  {
   match=i;
   bestPercent=percent;
  }
 }

 if (match!=-1) return patternOrder[match];
 else return Order(-1, typeNone);
}

void MousePattern::addSample(int x, int y)
{
 if (recording)
 {
  rawSampling.push_back(GVector<2>({double(x),double(y)}));
 }
}

void initMousePatternDatabase()
{
 #define ADDPAT(name, order) anglePatterns.push_back({F(0), F(1), F(2), F(3), F(4), F(5), F(6), F(7), F(8), F(9), F(10), F(11), F(12), F(13), F(14), F(15), F(16), F(17), F(18), F(19)}); patternName.push_back(name); patternOrder.push_back(order);

 for (unsigned int i=0; i<8; i++)
 {
  #define F(x) (i*M_PI/4)
  ADDPAT(dirName[i], Order(i, typeSimple));
  #undef F

  #define F(x) (x<nbSamples/2 ? i*M_PI/4 : i*M_PI/4+M_PI)
  ADDPAT(dirName[i]+"'", Order(i, typeReverse));
  #undef F

  #define F(x) ((x<nbSamples/3) || (x>2*nbSamples/3) ? i*M_PI/4 : i*M_PI/4+M_PI)
  ADDPAT(dirName[i]+"''", Order(i, typeDouble));
  #undef F

  #define F(x) (x<nbSamples/2 ? i*M_PI/4-M_PI/8 : i*M_PI/4-7*M_PI/8)
  ADDPAT(dirName[i]+" arrow", Order(i, typeStrong));
  #undef F

  #define F(x) (x<nbSamples/2 ? i*M_PI/4+M_PI/8 : i*M_PI/4+7*M_PI/8)
  ADDPAT(dirName[i]+" arrow", Order(i, typeStrong));
  #undef F

  #define F(x) (i*M_PI/4-M_PI/(nbSamples-1)*x)
  ADDPAT(dirName[i]+" arc", Order(i, typeComplex));
  #undef F

  #define F(x) (i*M_PI/4+M_PI/(nbSamples-1)*x)
  ADDPAT(dirName[i]+" arc", Order(i, typeComplex));
  #undef F

  #define F(x) ((x<nbSamples/3) || (x>2*nbSamples/3) ? i*M_PI/4 : i*M_PI/4+5*M_PI/6)
  ADDPAT(dirName[i]+" zig", Order(i, typeComplex));
  #undef F

  #define F(x) ((x<nbSamples/3) || (x>2*nbSamples/3) ? i*M_PI/4 : i*M_PI/4-5*M_PI/6)
  ADDPAT(dirName[i]+" zig", Order(i, typeComplex));
  #undef F

  #define F(x) (i*M_PI/4+M_PI/2-x*2*M_PI/(nbSamples-1))
  ADDPAT(dirName[i]+" circle", Order(i, typeComplex));
  #undef F

  #define F(x) (i*M_PI/4-M_PI/2+x*2*M_PI/(nbSamples-1))
  ADDPAT(dirName[i]+" circle", Order(i, typeComplex));
  #undef F

  #define F(x) (i*M_PI/4+M_PI/2-cos((1-cos(x*M_PI/(nbSamples-1)))*M_PI/2)*3*M_PI/4)
  ADDPAT(dirName[i]+" cross", Order(i, typeComplex));
  #undef F

  #define F(x) (i*M_PI/4-M_PI/2+cos((1-cos(x*M_PI/(nbSamples-1)))*M_PI/2)*3*M_PI/4)
  ADDPAT(dirName[i]+" cross", Order(i, typeComplex));
  #undef F
 }


 for (unsigned int iP=0; iP<anglePatterns.size(); iP++)
 {
  vecPatterns.push_back(array<GVector<2>, nbSamples>());
  for (unsigned int iS=0; iS<nbSamples; iS++)
  {
   vecPatterns.back()[iS]=GVector<2>({cos(anglePatterns[iP][iS]), sin(anglePatterns[iP][iS])});
  }
 }

 #undef ADDPAT
}
