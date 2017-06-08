#pragma once
#include "Chromosome.h"

Chromosome oc(int x, int y, const Chromosome& prt1, const Chromosome& prt2);
void mutate(Chromosome& c, double mutation_rate);
Chromosome roulette(int pop_size, const vector<Chromosome>& ch_vec);
bool cmp(const Chromosome& c1, const Chromosome& c2);