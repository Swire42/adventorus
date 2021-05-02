#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#define SHOW_CONTINENTS 1
int what2show=SHOW_CONTINENTS;

void initWindow();

void displayer(void (*drawFunc)());

void setDisplayMode();

void showWorld();

#endif // DISPLAY_HPP
