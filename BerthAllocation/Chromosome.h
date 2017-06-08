#pragma once
#pragma once
#include <vector>
#include "ConstDef.h"
#include "Ship.h"
using std::vector;

struct Chromosome {
  vector<int> order;
  vector<Ship> ori_ship;
  int penalty;
  Chromosome() {}
  Chromosome(vector<int> v, vector<Ship> ori) :order(v), ori_ship(ori), penalty(inf) {}
  Chromosome(vector<int> v, vector<Ship> ori, int penalty) :order(v), ori_ship(ori), penalty(penalty) {}
};