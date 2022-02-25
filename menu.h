#ifndef __MENU_H__
#define __MENU_H__

#include "simulation.h"

#define ENTER_PIN 12
#define NEXT_PIN 11
#define PREV_PIN 10

void initMenu();
bool loopMenu(unsigned long now);

#endif // __MENU_H__
