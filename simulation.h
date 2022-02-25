#ifndef __SIMULATION_H__
#define __SIMULATION_H__

#include "global.h"

#define SIM_STEP 100
//#define WRAP_I
//#define WRAP_J

extern fixed cellsA[2][SIM_DIM][SIM_DIM];
extern fixed cellsB[2][SIM_DIM][SIM_DIM];

extern fixed diffA;
extern fixed feedA;
extern fixed dripA;

extern fixed diffB;
extern fixed killB;
extern fixed dripB;

void initSimulation();
void loopSimulation(unsigned long now);

#endif //  __SIMULATION_H__
