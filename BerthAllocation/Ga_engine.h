#pragma once
#include "Chromosome.h"
#include "Alloc_list.h"
#include "Ba_func.h"
#include <cmath> 
class Ga_engine
{
public:
  void Ga_engine::init(int ppsize, int genaration, double mutationrate, const vector<Ship>& ship);
  Alloc_list begin();
  void bestfitness();
  void epoch();
  Alloc_list allo_with_cm(Chromosome cm);
  Chromosome get_best() {
    return best_cm;
  }
private:
  vector<Chromosome> ch_vec;
  int pop_size;
  int genaration;
  double mutation_rate;
  static int best;
  static Chromosome best_cm;
};


