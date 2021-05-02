#include <iostream>
#include <vector>
#include <array>

#include "EasySDL/v6/EasySDL.hpp"
#include "Geometry/v2/geometry.hpp"

#include "mousePattern.hpp"
#include "kpPattern.hpp"
#include "player.hpp"

using namespace std;

const int winSize=700;
const int centerX=winSize/2, centerY=winSize/2;

MousePattern mousePattern;
KPPattern kpPattern;

Player p1("P1"), p2("P2");

void startRec(int x, int y) {mousePattern.startRec();}
void stopRec(int x, int y)
{
 Order o=mousePattern.validate();
 if (o.type!=typeNone) p1.pushOrder(o);
}
void addSample(int x, int y)
{
 if ((x==centerX) && (y==centerY)) return;
 mousePattern.addSample(x-centerX, centerY-y);
 SDL_WarpMouse(centerX, centerY);
}

void kp1() {kpPattern.input(dirSW);}
void kp2() {kpPattern.input(dirS);}
void kp3() {kpPattern.input(dirSE);}
void kp4() {kpPattern.input(dirW);}
void kp5() {kpPattern.input(-1);}
void kp6() {kpPattern.input(dirE);}
void kp7() {kpPattern.input(dirNW);}
void kp8() {kpPattern.input(dirN);}
void kp9() {kpPattern.input(dirNE);}

void p1ShieldOn(int x, int y) {p1.pushOrder(Order(shOn, typeShield));}
void p1ShieldOff(int x, int y) {p1.pushOrder(Order(shOff, typeShield));}
void p2ShieldOn() {p2.pushOrder(Order(shOn, typeShield));}
void p2ShieldOff() {p2.pushOrder(Order(shOff, typeShield));}

int main()
{
 initEasySDL(winSize, winSize, "Pattern Match");
 SDL_EnableKeyRepeat(0, 0);
 TTF_Font *font=NULL;
 text::load("font.ttf", 50, &font);

 SDL_Cursor *cursor;
 int32_t cursorData[2] = {0, 0};
 cursor = SDL_CreateCursor((Uint8 *)cursorData, (Uint8 *)cursorData, 8, 8, 4, 4);
 SDL_SetCursor(cursor);

 mouseButtonLeftDown=startRec;
 mouseButtonLeftUp=stopRec;
 mouseMotion=addSample;
 mouseButtonRightDown=p1ShieldOn;
 mouseButtonRightUp=p1ShieldOff;

 keyDown[SDLK_KP1]=kp1;
 keyDown[SDLK_KP2]=kp2;
 keyDown[SDLK_KP3]=kp3;
 keyDown[SDLK_KP4]=kp4;
 keyDown[SDLK_KP5]=kp5;
 keyDown[SDLK_KP6]=kp6;
 keyDown[SDLK_KP7]=kp7;
 keyDown[SDLK_KP8]=kp8;
 keyDown[SDLK_KP9]=kp9;
 keyDown[SDLK_KP_ENTER]=p2ShieldOn;
 keyUp[SDLK_KP_ENTER]=p2ShieldOff;


 initMousePatternDatabase();

 p1.enemy=&p2;
 p2.enemy=&p1;
 while (true)
 {
  window::events();
  if (quit) break;

  if (kpPattern.isFinished())
  {
   Order o=kpPattern.get();
   if (o.type!=typeNone) p2.pushOrder(o);
  }

  p1.tick();
  p2.tick();

  p1.display();
  p2.display();

  window::refresh();
  waitFromLastWait(30);
 }

 SDL_FreeCursor(cursor);
}
