/*
* The algorithms of Genetic algorithm 
*/
#include <cmath>
#include "Genetic.h"

/*
* order cross function
* @x,left position that will be preserved
* @y,left position that will be preserved
* @part1,part2 two Chromosome that to be order cross
*/
Chromosome oc(int x, int y, const Chromosome& prt1, const Chromosome& prt2)
{
  vector<int> subs;
  subs.resize(prt1.order.size());
  int cnt1 = 0, cnt2 = y + 1;
  for (int i = x; i <= y; i++) {
    subs[i] = prt1.order[i];
  }
  for (int i = 0; i<prt1.ori_ship.size(); i++) {
    int flag = 0;
    for (int j = x; j <= y; j++) {
      if (prt2.order[i] == prt1.order[j]) {
        flag = 1;
        break;
      }
    }
    if (!flag) {
      if (cnt1<x) {
        subs[cnt1++] = prt2.order[i];
      }
      else subs[cnt2++] = prt2.order[i];
    }
  }
  return Chromosome(subs,prt1.ori_ship);
}

/*
* mutate function
* @c the Chromosome that to be mutate
* @mutation_rate, frequency of mutate
*/
void mutate(Chromosome & c, double mutation_rate)
{
  vector<int> re;
  re.resize(c.order.size());

  int rd = rand();
  if (mutation_rate > (double)rd / RAND_MAX) { //随机变异
    for (int i = 0; i < c.order.size(); i++) {
      re[i] = -1;
    }
    for (int i = 0; i < c.order.size(); i++) {
      int t = rand() % c.order.size();
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
  }
  else re = c.order;
  c.order = re;
}

/*
* roulette function
* @pop_size, totle population size
* @ch_vec, store the totle population
*/
Chromosome roulette(int pop_size, const vector<Chromosome>& ch_vec)
{
  int flag = 0;
  double part = rand();
  part = part / RAND_MAX;
  part *= pop_size; //大小控制在人口范围内
  //part = log(part + 1);
  part = (int)part;
  if (part < 0)part = 0;
  if (part >= ch_vec.size())part = ch_vec.size() - 1;
  return ch_vec[part];
}

bool cmp(const Chromosome& c1, const Chromosome& c2) {
  return c1.penalty < c2.penalty;
}