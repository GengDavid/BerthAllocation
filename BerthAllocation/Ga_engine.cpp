#include <algorithm>
#include <cmath>
#include "Ga_engine.h"
#include "Ba_func.h"
#include "Genetic.h"

using std::sort;
using std::vector;

int Ga_engine::best = inf;
Chromosome Ga_engine::best_cm = Chromosome();

void m_swap(int& a, int &b) {
  int t = a;
  a = b;
  b = t;
}

/*
* init ga_engine
* @ppsize, totoal population size
* @genaration, totole iteration times
* @mutationrate, specify the mutation rate
* @ship, oringinal ship condition
*/
void Ga_engine::init(int ppsize, int genaration, double mutationrate, const vector<Ship>& ship)
{
  pop_size = ppsize;
  this->genaration = genaration;
  mutation_rate = mutationrate;
  int ssize = ship.size();

  /*initialize population*/
  for (int p = 0; p < pop_size; p++) {
    vector<int> re;
    re.resize(ssize);

    for (int i = 0; i < ssize; i++) {
      re[i] = -1;
    }
    for (int i = 0; i < ssize; i++) {
      int t = rand() % ssize;
      int flag = 0;
      for (int j = 0; j < i; j++) {
        if (t == re[j]) {
          flag = 1;
          break;
        }
      }
      if (flag) {
        i--;
        continue;
      }
      re[i] = t;
    }

    ch_vec.push_back(Chromosome(re, ship));
  }
}

/*
* function that control the population evolution
* return val: an allocation scheme
*/
Alloc_list Ga_engine::begin()
{
  Alloc_list best_alloc_list;

  /*alloc with the first generation*/
  for (int j = 0; j < ch_vec.size(); j++) {
    Alloc_list temp_allo = allo_with_cm(ch_vec[j]);
    ch_vec[j].penalty = evaluate(temp_allo, ch_vec[j].ori_ship);
  }

  /*iteration genaration times*/
  for (int i = 0; i < genaration; i++) {   
    /*evolution*/
    sort(ch_vec.begin(), ch_vec.end(), cmp);
    epoch();   
    /*alloc with the next generation*/
    for (int j = 0; j < ch_vec.size(); j++) {
      Alloc_list temp_allo = allo_with_cm(ch_vec[j]);
      ch_vec[j].penalty = evaluate(temp_allo, ch_vec[j].ori_ship);
      if (temp_allo.bad_allo < best_alloc_list.bad_allo) best_alloc_list = temp_allo;
    }
    if (best_alloc_list.bad_allo == 0) {
      break;
    }
    ch_vec.resize(pop_size);
    printf("%d\n", i);
  }
  return best_alloc_list;
}

void Ga_engine::bestfitness()
{
  for (int i = 0; i < ch_vec.size(); i++) {
    if (ch_vec[i].penalty < best) {
      best = ch_vec[i].penalty;
      best_cm = ch_vec[i];
    }
  }
}

/*
* function that make population evolute
*/
void Ga_engine::epoch()
{
  vector<Chromosome> new_ch_vec;
  while (new_ch_vec.size() < ch_vec.size()) {
    /*chose two Chromosome to generate next Chromosome*/
    Chromosome mum = roulette(pop_size, ch_vec);
    Chromosome dad = roulette(pop_size, ch_vec);
    Chromosome baby1, baby2;
    int ship_num = (int)mum.ori_ship.size();
    int ok = 0, x, y;
    x = rand() % ship_num;
    y = rand() % ship_num;
    if (x > y)m_swap(x, y);
    baby1 = oc(x, y, mum, dad);
    baby2 = oc(x, y, dad, mum);

    /*mutation with mutation rate*/
    mutate(baby1, mutation_rate);
    mutate(baby2, mutation_rate);
    new_ch_vec.push_back(baby1);
    new_ch_vec.push_back(baby2);
  }
}

/*
* allocate berth with a chromosome
* return val: an allocation scheme
*/
Alloc_list Ga_engine::allo_with_cm(Chromosome cm) {
  vector<Ship> temp;
  for (size_t i = 0; i < cm.order.size(); i++) {
    temp.push_back(cm.ori_ship[cm.order[i]]);
  }
  /*using greedy to allocate according to the order*/
  return greedy(temp);
}
