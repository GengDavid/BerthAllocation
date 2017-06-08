#pragma once
#include <vector>
using std::vector;

struct Chromosome {
  vector<int> order;
  int grade;
  int penalty;
};