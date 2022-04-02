#include <iostream>
#include <string.h>
#include <cmath>
#include <chrono>
#include <conio.h>

static float brightness[2][32][32] = {};
static int occupancy[32][32] = {
  { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
  { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
  { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
  { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
  { 0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,},
  { 0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,},
  { 0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,},
  { 0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,},
  { 0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,},
  { 0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,},
  { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,},
  { 0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,},
  { 0,0,1,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,1,0,0,1,1,1,1,1,1,1,0,},
  { 0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,},
  { 0,0,1,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,},
  { 0,0,1,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,},
  { 0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,},
  { 0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,},
  { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,},
  { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
  { 0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,},
  { 0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,},
  { 0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,},
  { 0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,},
  { 0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,},
  { 0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,},
  { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
  { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
  { 0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,},
  { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
  { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
  { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
};
//  { 0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
//  { 0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
//  { 0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
//  { 0,0,0,0,0,0,0,1,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,},
//  { 0,0,0,1,1,1,1,1,1,0,1,1,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,},
//  { 0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,},
//  { 0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,},
//  { 0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,},
//  { 0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,},
//  { 0,0,0,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,},
//  { 0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,},
//  { 0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,},
//  { 1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,},
//  { 1,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,},
//  { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,},
//  { 1,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,},
//  { 1,1,1,1,1,0,0,0,0,0,0,0,1,1,1,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,},
//  { 0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,},
//  { 0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,},
//  { 0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,},
//  { 0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,},
//  { 0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,},
//  { 0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,},
//  { 0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,},
//  { 0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,},
//  { 0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,},
//  { 0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,},
//  { 0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,},
//  { 0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,},
//  { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
//  { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
//  { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
//};

static int fog[32][32] = {
  { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
  { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
  { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
  { 1,0,0,0,0,0,0,2,2,2,1,1,1,1,2,0,0,0,0,0,0,0,2,2,2,1,1,1,1,2,},
  { 1,0,0,0,0,0,1,3,4,2,4,4,4,4,4,0,0,0,0,0,0,1,3,4,2,4,4,4,4,4,},
  { 1,1,0,0,0,0,2,3,4,4,4,4,4,4,2,0,0,0,0,0,0,2,3,4,4,4,4,4,4,2,},
  { 1,1,0,0,0,0,0,3,4,4,4,4,4,2,2,0,1,1,0,0,0,0,3,4,4,4,4,4,2,2,},
  { 1,1,1,1,0,0,0,3,4,4,4,3,3,1,0,1,2,1,0,0,0,0,3,4,4,4,3,3,1,0,},
  { 1,1,1,1,1,0,0,3,3,3,4,3,2,1,0,1,2,1,0,0,0,0,3,3,3,4,3,2,1,0,},
  { 1,1,1,1,1,0,0,0,3,2,3,3,1,1,1,1,1,2,1,0,0,0,0,3,2,3,3,1,0,0,},
  { 1,1,1,0,1,1,0,0,2,2,3,3,1,1,0,0,0,2,2,1,0,0,0,2,2,3,3,1,0,0,},
  { 1,1,1,1,0,1,0,0,2,3,3,3,1,2,0,0,0,1,2,1,0,0,0,2,3,3,3,1,0,0,},
  { 1,1,1,1,0,0,0,0,4,2,3,1,2,3,1,1,1,3,2,1,0,0,0,4,2,3,1,0,0,0,},
  { 1,1,1,1,1,1,0,0,3,2,2,1,1,4,2,3,1,1,0,1,0,0,0,3,2,0,0,0,0,0,},
  { 1,1,1,1,0,1,0,0,1,1,1,4,4,4,4,3,4,2,0,0,0,0,0,1,1,0,0,0,0,0,},
  { 1,1,1,1,1,0,0,0,0,0,0,1,2,2,3,3,1,1,0,0,0,0,0,0,0,0,0,0,0,0,},
  { 1,1,1,1,0,0,0,0,0,0,0,0,1,1,3,3,2,0,0,0,0,0,0,0,0,0,0,0,0,0,},
  { 1,1,1,0,0,1,0,0,0,0,0,0,0,0,1,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
  { 1,1,1,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
  { 1,1,1,0,1,1,0,2,2,2,1,1,1,1,2,0,0,0,0,0,0,0,2,2,2,1,1,1,1,2,},
  { 1,1,1,0,1,0,1,3,4,2,4,4,4,4,4,0,0,0,0,0,0,1,3,4,2,4,4,4,4,4,},
  { 1,1,0,1,0,0,2,3,4,4,4,4,4,4,2,0,0,0,0,0,0,2,3,4,4,4,4,4,4,2,},
  { 1,1,0,1,1,1,0,3,4,4,4,4,4,2,2,0,0,0,0,0,0,0,3,4,4,4,4,4,2,2,},
  { 1,0,1,1,1,0,0,3,4,4,4,3,3,1,0,0,0,0,0,0,0,0,3,4,4,4,3,3,1,0,},
  { 1,0,1,0,1,1,0,3,3,3,4,3,2,1,0,0,0,0,0,0,0,0,3,3,3,4,3,2,1,0,},
  { 1,0,1,0,0,0,0,0,3,2,3,3,1,0,0,0,0,0,0,0,0,0,0,3,2,3,3,1,0,0,},
  { 0,1,1,1,0,0,0,0,2,2,3,3,1,0,0,0,0,0,0,0,0,0,0,2,2,3,3,1,0,0,},
  { 1,1,0,1,0,0,0,1,2,3,3,3,1,0,0,0,0,0,0,0,0,0,0,2,3,3,3,1,0,0,},
  { 0,0,0,1,0,1,1,1,4,2,3,1,0,0,0,0,0,0,0,0,0,0,0,4,2,3,1,0,0,0,},
  { 1,1,1,0,0,1,1,0,3,2,0,0,0,0,0,0,0,0,0,0,0,0,0,3,2,0,0,0,0,0,},
  { 0,1,1,1,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,},
  { 0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
};
//  { 4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,3,4,3,4,4,4,4,4,4,4,4,3,3,4,},
//  { 4,4,4,4,4,4,4,4,3,4,4,4,4,4,4,3,4,3,3,4,4,4,4,4,4,3,3,3,3,4,},
//  { 4,4,4,4,4,3,3,4,4,4,3,2,2,3,2,3,4,2,3,3,3,3,2,4,4,2,3,3,4,4,},
//  { 4,3,4,3,4,4,4,4,4,4,4,4,4,4,4,2,2,2,3,3,3,3,3,3,3,4,4,3,3,4,},
//  { 4,4,4,4,4,4,3,4,3,4,4,3,4,3,4,4,4,4,4,4,4,4,3,3,3,4,4,4,4,4,},
//  { 4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,3,3,2,2,2,4,},
//  { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
//  { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
//  { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
//  { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
//  { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
//  { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
//  { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
//  { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
//  { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
//  { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
//  { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
//  { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
//  { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
//  { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
//  { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
//  { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
//  { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
//  { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
//  { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
//  { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
//  { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
//  { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
//  { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
//  { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
//  { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
//  { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
//};

static int c = 0;

float fog_calc(int a) {
  return 1.0f - ((float)a * 0.15f);
}

char print_brightness(int c, int x, int y) {
  float b = brightness[c][x][y];
  int oc = occupancy[x][y];

  b /= 8.001f;
  b = powf(b, 1.0f / 2.2f);

  //if(oc > 0) {
  //    return 'D';
  //}

  //char* palette = " .:-=+*#%@";
  char* palette = "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\\|()1{}[]?-_+~<>i!lI;:,\"^`\'. ";
  return palette[strlen(palette) - (int)(b * (float)strlen(palette))];
}

void print_all() {
  printf(
      " Lighting                                                       "
      "|"
      " Walls                                                           "
      "|"
      " Fog\n");

  std::string output;

  for(long x = 0; x < 32; x += 1) {
    for(long y = 0; y < 32; y += 1) {
      output.push_back(print_brightness(c, x, y));
      output.append(" ");
    }

    output.append("| ");

    for(long y = 0; y < 32; y += 1) {
      if(occupancy[x][y] == 1) {
        output.append("1");
      } else {
        output.append(" ");
      }
      output.append(" ");
    }

    output.append("| ");

    for(long y = 0; y < 32; y += 1) {
      if(fog[x][y] != 0) {
        char buffer[3];
        sprintf(buffer, "%d", fog[x][y]);
        output.append(buffer);
      } else {
        output.append(" ");
      }
      output.append(" ");
    }
    output.append("\n");
  }

  std::cout << output;
  //printf("%s\n", output.c_str());
}

bool bounds(long x, long y) {
  return x >= 0 && y >= 0 && x < 32 && y < 32;
}

void propegate_brightness() {
  for(long x = 0; x < 32; x += 1) {
    for(long y = 0; y < 32; y += 1) {
      long wx = x;
      long wy = y;

      float b = 0.0f;

      wx = x - 1; wy = y;
      if(bounds(wx, wy)) {
        b += fog_calc(fog[wx][wy]) * brightness[c][wx][wy];
      }

      //wx = x - 1; wy = y + 1;
      //if(bounds(wx, wy)) {
      //  b += brightness[c][wx][wy];
      //}

      wx = x; wy = y + 1;
      if(bounds(wx, wy)) {
        b += fog_calc(fog[wx][wy]) * brightness[c][wx][wy];
      }

      //wx = x + 1; wy = y + 1;
      //if(bounds(wx, wy)) {
      //  b += brightness[c][wx][wy];
      //}

      wx = x + 1; wy = y;
      if(bounds(wx, wy)) {
        b += fog_calc(fog[wx][wy]) * brightness[c][wx][wy];
      }

      //wx = x + 1; wy = y - 1;
      //if(bounds(wx, wy)) {
      //  b += brightness[c][wx][wy];
      //}

      wx = x; wy = y - 1;
      if(bounds(wx, wy)) {
        b += fog_calc(fog[wx][wy]) * brightness[c][wx][wy];
      }

      //wx = x - 1; wy = y - 1;
      //if(bounds(wx, wy)) {
      //  b += brightness[c][wx][wy];
      //}

      b /= 4.0f;

      if(occupancy[x][y] <= 0) {
        brightness[(c + 1) % 2][x][y] = b;
      }
    }
  }

  c += 1;
  c %= 2;
}

bool is_equilibrium() {
  for(int x = 0; x < 32; x += 1) {
    for(int y = 0; y < 32; y += 1) {
      if(brightness[0][x][y] != brightness[1][x][y]) {
        return false;
      }
    }
  }

  return true;
}

int main() {
  int px = 4;
  int py = 6;
  int i = 0;

  brightness[c][px][py] = 8.0f;

  i = 0;
  while(i < 100) {
    propegate_brightness();
    brightness[c][px][py] = 8.0f;
    i += 1;
  }
  print_all();

  using namespace std::chrono;

  char input = ' ';
  while(input != 'q') {
    bool valid = false;

    input = getch();

    switch(input) {
    case('a'): {
      if(occupancy[px][py-1] <= 0) { py -= 1; }
      valid = true;
    } goto VALID;
    case('w'): {
      if(occupancy[px-1][py] <= 0) {  px -= 1; }
      valid = true;
    } goto VALID;
    case('d'): {
      if(occupancy[px][py+1] <= 0) { py += 1; }
      valid = true;
    } goto VALID;
    case('s'): {
      if(occupancy[px+1][py] <= 0) { px += 1; }
      valid = true;
    } goto VALID;
    default: continue;
    }

VALID:

    auto t0 = high_resolution_clock::now();
    i = 0;

    for(int x = 0; x < 32; x += 1) {
      for(int y = 0; y < 32; y += 1) {
        brightness[0][x][y] = 0.0f;
        brightness[1][x][y] = 0.0f;
      }
    }

    brightness[c][px][py] = 8.0f;

    while(i < 100) {
      propegate_brightness();
      brightness[c][px][py] = 8.0f;
      i += 1;
    }
    auto t1 = high_resolution_clock::now();

    printf("Time: %f\n", duration_cast<duration<float>>(t1 - t0).count() * 1000.0f);
    printf("%d\n", i);
    print_all();
  }
}
