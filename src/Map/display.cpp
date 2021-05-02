#include "display.hpp"
#include <iostream>
#include "EasySDL/EasySDL.cpp"

void initWindow()
{
 initEasySDL(512, 512, "Displayer");
}

bool continueDisplay;
void stopDisplay()
{
 continueDisplay=false;
}

void displayer(void (*drawFunc)())
{
 associations::associateDown(SDLK_ESCAPE, stopDisplay);
 continueDisplay=true;
 while (continueDisplay)
 {
  drawFunc();

  window::events();
  window::refresh();
  waitFromLastWait(30);
 }
}



setDisplayMode(int _mode)
{

}
