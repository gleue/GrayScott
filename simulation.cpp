#include "simulation.h"
#include "display.h"
#include <EEvar.h>

fixed cellsA[2][SIM_DIM][SIM_DIM];
fixed cellsB[2][SIM_DIM][SIM_DIM];

fixed diffA;
fixed feedA;
fixed dripA;

fixed diffB;
fixed killB;
fixed dripB;

const EEstore<float> eeDiffA = 1.0;
const EEstore<float> eeFeedA = 0.055;
const EEstore<float> eeDripA = 0.05;

const EEstore<float> eeDiffB = 0.5;
const EEstore<float> eeKillB = 0.062;
const EEstore<float> eeDripB = 0.01;

void loadParameters() {
  float value;

  eeDiffA >> value;
  diffA = value;
  eeFeedA >> value;
  feedA = value;
  eeDripA >> value;
  dripA = value;

  eeDiffB >> value;
  diffB = value;
  eeKillB >> value;
  killB = value;
  eeDripB >> value;
  dripB = value;
}

void saveParameters() {
  float value;

  value = static_cast<float>(diffA);
  eeDiffA << value;
  value = static_cast<float>(feedA);
  eeFeedA << value;
  value = static_cast<float>(dripA);
  eeDripA << value;

  value = static_cast<float>(diffB);
  eeDiffB << value;
  value = static_cast<float>(killB);
  eeKillB << value;
  value = static_cast<float>(dripB);
  eeDripB << value;
}

void initCells(uint8_t idx, uint8_t seedCount = 2) {
  for (uint8_t i = 0; i < SIM_DIM; i++) {
    for (uint8_t j = 0; j < SIM_DIM; j++) {
      cellsA[idx][i][j] = 1.0;
      cellsB[idx][i][j] = 0.0;
    }
  }

  while (seedCount > 0) {
    int si = random(0, SIM_DIM);
    int sj = random(0, SIM_DIM);
    
    if (cellsB[idx][si][sj] != 0.0) continue;
    
    Serial.print(F("Seed ")); Serial.print(si); Serial.print(' '); Serial.println(sj);

    cellsA[idx][si][sj] = 0.0;
    cellsB[idx][si][sj] = 1.0;
    seedCount -= 1;
  }
}

void dripCells(uint8_t idx) {
  if (float(random(0,1024)) / 1023.0 < dripA) {
    int di = random(0, SIM_DIM);
    int dj = random(0, SIM_DIM);
    
    Serial.print(F("Drip A: ")); Serial.print(di); Serial.print(' '); Serial.println(dj);

    cellsA[idx][di][dj] = 1.0;
  }

  if (float(random(0,1024)) / 1023.0 < dripB) {
    int di = random(0, SIM_DIM);
    int dj = random(0, SIM_DIM);
    
    Serial.print(F("Drip B: ")); Serial.print(di); Serial.print(' '); Serial.println(dj);

    cellsB[idx][di][dj] = 1.0;
  }
}

void showCells(uint8_t idx) {
  drawPixels(cellsA[idx], cellsB[idx]);
}

//fixed weights[][3] = {{.05, .2, .05}, {.2, 0.0, .2}, {.05, .2, .05}};
const fixed weights[][3] = {{.7, 1., .7}, {1., 0.0, 1.}, {.7, 1., .7}};

fixed laplacian(fixed cells[SIM_DIM][SIM_DIM], uint8_t i, uint8_t j) {
  fixed sum = 0.0;
  fixed weightSum = 0.0;
  for (uint8_t ii = 0; ii < 3; ii++) {
#ifdef WRAP_I
    uint8_t ci = (SIM_DIM + i + ii - 1) % SIM_DIM;
#else
    int8_t ci = i + ii - 1;
    if (ci < 0 || ci >= SIM_DIM) continue;
#endif
    for (uint8_t jj = 0; jj < 3; jj++) {
      if (ii == 0 && jj == 0) continue;
#ifdef WRAP_J
      uint8_t cj = (SIM_DIM + j + jj - 1) % SIM_DIM;
#else
      int8_t cj = j + jj - 1;
      if (cj < 0 || cj >= SIM_DIM) continue;
#endif
      fixed w = weights[ii][jj];
      weightSum += w;
      sum += w * cells[ci][cj];
    }
  }
  return sum / weightSum - cells[i][j];
}

unsigned long last_time;
uint8_t cur;

void initSimulation() {
  Serial.println(F("Init"));
  loadParameters();
  resetSimulation();
}

void loopSimulation(unsigned long now) {

  if (now - last_time >= SIM_STEP) {
    uint8_t nxt = (cur + 1) % 2;
    fixed dt = float(now - last_time) / 1000.0;

    for (uint8_t i = 0; i < SIM_DIM; i++) {
      for (uint8_t j = 0; j < SIM_DIM; j++) {
        fixed L2A = laplacian(cellsA[cur], i, j);
        fixed L2B = laplacian(cellsB[cur], i, j);
        fixed A = cellsA[cur][i][j];
        fixed B = cellsB[cur][i][j];
        
        cellsA[nxt][i][j] = constrain(A + (diffA * L2A - A*B*B + feedA * (1.0 - A)) * dt, 0.0, 1.0);
        cellsB[nxt][i][j] = constrain(B + (diffB * L2B + A*B*B - (killB + feedA) * B) * dt, 0.0, 1.0);
      }
    }

    dripCells(nxt);
    showCells(nxt);

    last_time = now;
    cur = nxt;
  }
}

void resetSimulation() {
  Serial.println(F("Reset"));
  cur = 0;
  initCells(cur, random(1,4));
  showCells(cur);
}
