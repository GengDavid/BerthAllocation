#pragma once
#include <vector>
#include "ConstDef.h"

using std::vector;

struct Allocated {
  int leftx, lefty;
  int rightx, righty;
  int id;
  Allocated(int lx, int ly, int rx, int ry, int id)
    :leftx(lx), lefty(ly), rightx(rx), righty(ry), id(id) {}
};
struct Alloc_list {
  vector<Allocated> allocated;
  int bad_allo;
  int pen;
  int ttwt;
  int lstdpt;
  Alloc_list(vector<Allocated> allo, int bad_allo)
    :allocated(allo), bad_allo(bad_allo) {}
  Alloc_list() :bad_allo(inf){}
};
