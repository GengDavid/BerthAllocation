#pragma once
#include <vector>
#include "Alloc_list.h"
#include "Ship.h"

using std::vector;

struct Loss {
  int bad_allo;
  int ttwt;
  int lstdpt;
  int pen;
  Loss(int bd, int tt, int lst, int pen) :bad_allo(bd), ttwt(tt), lstdpt(lst), pen(pen) {}
};

bool is_num(char c);

bool judge(const vector<Allocated>& allo, int lx, int ly, int rx, int ry);

int evaluate(const Alloc_list& alloc_list, const vector<Ship>& ship);
Loss evaluate2(const Alloc_list& alloc_list, const vector<Ship>& ship);

Alloc_list greedy(const vector<Ship>& ship);

bool allo_cmp(const Allocated& a1, const Allocated& a2);