#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <cstring>
#include <vector>
#include <ctime>
#include <algorithm>
#include <functional>

using std::ifstream;
using std::ofstream;
using std::string;
using std::stringstream;
using std::cout;
using std::endl;
using std::vector;
using std::lower_bound;
class Ship {
public:
  Ship() {}
  Ship(int id, int arv, int svc, int berth) 
    :id(id), arv_tm(arv), svc_tm(svc), berth_occupy(berth) {}

  int id;
  int arv_tm;
  int svc_tm;
  int berth_occupy;
}**ship;

int n, m;
const int infinit = 0x7fffffff;

struct Allocated {
  int leftx, lefty;
  int rightx, righty;
  int id;
};

bool is_num(char c) {
  return c >= '0'&&c <= '9';
}

void judge(Allocated* allo,int allo_cnt, int lx, int ly, int rx, int ry, int &overlap) {
  for (int j = 0; j < allo_cnt; j++) {
    if ((rx >= allo[j].leftx&&rx <= allo[j].rightx&&ry >= allo[j].lefty&&ry <= allo[j].righty) ||
      (rx >= allo[j].leftx&&rx <= allo[j].rightx&&ly >= allo[j].lefty&&ly <= allo[j].righty) ||
      (lx >= allo[j].leftx&&lx <= allo[j].rightx&&ry >= allo[j].lefty&&ry <= allo[j].righty) ||
      (lx >= allo[j].leftx&&lx <= allo[j].rightx&&ly >= allo[j].lefty&&ly <= allo[j].righty) ||
      (allo[j].rightx >= lx&&allo[j].rightx <= rx&&allo[j].righty >= ly&&allo[j].righty <= ry) ||
      (allo[j].rightx >= lx&&allo[j].rightx <= rx&&allo[j].lefty >= ly &&allo[j].lefty <= ry) ||
      (allo[j].leftx >= lx&&allo[j].leftx <= rx&&allo[j].righty >= ly&&allo[j].righty <= ry) ||
      (allo[j].leftx >= lx&&allo[j].leftx <= rx&&allo[j].lefty >= ly&&allo[j].lefty <= ry))
      overlap = 1;
  }
}

struct Chromosome {
  int *position;
  int *tm;
  int *order;
  int fitness;
  Chromosome() :position(nullptr), tm(nullptr), order(nullptr), fitness(0) {}
};

class Ga_engine {
public:
  int * oc(int x, int y, int * prt1, int * prt2);
  int* mutate(int * ori, int base);
  void init(int n, int m, int num);
  Chromosome roulette();
  Allocated * allo_with_cm(Chromosome cm, int & allo_cnt, int & bad_allo);
  Chromosome ga(int n, int m, int num);
  Ga_engine(int pop_size, int gen) :pop_size(pop_size), generation(gen) {}
  void epoch();
  int bestfitness();
private:
  vector<Chromosome> cm_vec;
  int pop_size;
  int totfit;
  int generation;
  int ship_num;
  int mutation_rate;
  Chromosome best_cm;
};


int evaluate(Allocated*& allo, int allo_cnt, int bad_allo) {
  int ttwt = 0, last_departure_tm = -1;
  for (int i = 0; i < allo_cnt; i++) {
    ttwt += allo[i].lefty - ship[allo[i].id]->arv_tm;
    if (allo[i].righty + 1 > last_departure_tm)last_departure_tm = allo[i].righty + 1;
  }
  //cout << bad_allo << " " << ttwt << " " << last_departure_tm << endl;
  return bad_allo * 100 + ttwt * 10 + last_departure_tm * 10;
}

int* Ga_engine::oc(int x, int y, int* prt1, int* prt2) {
  int* subs = new int[ship_num];
  int cnt1 = 0, cnt2 = y + 1;
  for (int i = x; i <= y; i++) {
    subs[i] = prt1[i];
  }
  for (int i = 0; i<ship_num; i++) {
    int flag = 0;
    for (int j = x; j <= y; j++) {
      if (prt2[i] == prt1[j]) {
        flag = 1;
        break;
      }
    }
    if (!flag) {
      if (cnt1<x) {
        subs[cnt1++] = prt2[i];
      }
      else subs[cnt2++] = prt2[i];
    }
  }
  return subs;
}

int* Ga_engine::mutate(int *ori, int base) {
  int* re = new int[base];
  for (int i = 0; i < ship_num; i++) {
    int rd = rand();
    if (rd / RAND_MAX > mutation_rate) {
      rd = rd%base;
      rd = (int)(rd*0.5);
      rd = (int)(rd - 0.5);
      re[i] = ori[i] + rd;
      if (re[i] > base)re[i] = base;
      if (re[i] < 0)re[i] = 0;
    }
    else re[i] = ori[i];
  }
  return re;
}

void Ga_engine::init(int n, int m, int num)
{
  for (int p = 0; p < pop_size; p++) {
    Chromosome temp;
    temp.order = new int[num];
    for (int i = 0; i < num; i++) {
      temp.order[i] = -1;
    }
    for (int i = 0; i < num; i++) {
      int t = rand() % num;
      int flag = 0;
      for (int j = 0; j < i; j++) {
        if (t == temp.order[j]) {
          flag = 1;
          break;
        }
      }
      if (flag) {
        i--;
        continue;
      }
      temp.order[i] = t;
    }
    temp.position = new int[num];
    for (int i = 0; i < num; i++) {
      int t = rand() % n;
      temp.position[i] = t;
    }
    temp.tm = new int[num];
    for (int i = 0; i < num; i++) {
      int t = rand() % m;
      temp.tm[i] = t;
    }
    cm_vec.push_back(temp);
  }
  ship_num = num;
}

Chromosome Ga_engine::roulette()
{
  double part = rand()*1.0 / RAND_MAX;
  part *= totfit;
  Chromosome temp;
  double select = 0;
  for (int i = 0; i < pop_size; i++) {
    select += cm_vec[i].fitness;
    if (select >= part) {
      temp = cm_vec[i];
      break;
    }
  }
  return temp;
}

Allocated* Ga_engine::allo_with_cm(Chromosome cm, int& allo_cnt, int& bad_allo) {
  int* no = cm.order;
  int* pos = cm.position;
  int* tm = cm.tm;
  Allocated* allo = new Allocated[ship_num];
  for (int i = 0; i < ship_num; i++) {
    int od = no[i];
    int overlap = 0;
    int lx = pos[od], ly = tm[od];
    int rx = lx + ship[od]->berth_occupy;
    int ry = ly + ship[od]->svc_tm;
    if (ly<ship[i]->arv_tm || rx>n) {
      bad_allo++;
      continue;
    }
    judge(allo, allo_cnt, lx, ly, rx, ry, overlap);
    if (overlap) {
      bad_allo++;
      continue;
    }
    else {
      allo[allo_cnt].leftx = lx;
      allo[allo_cnt].lefty = ly;
      allo[allo_cnt].rightx = rx;
      allo[allo_cnt].righty = ry;
      allo[allo_cnt].id = ship[i]->id;
      allo_cnt++;
    }
  }
  return allo;
}

void Ga_engine::epoch()
{
  vector<Chromosome> new_cm_vec;
  while ((int)new_cm_vec.size() < pop_size) {
    Chromosome mum = roulette();
    Chromosome dad = roulette();
    Chromosome baby1, baby2;
    int ok = 0, x, y;
    while (!ok) {
      x = rand() % ship_num;
      y = rand() % ship_num;
      if (x < y)ok = 1;
    }
    baby1.order = oc(x, y, mum.order, dad.order);
    baby2.order = oc(x, y, dad.order, mum.order);
    baby1.position = mutate(mum.position,n);
    baby2.position = mutate(dad.position,n);
    baby1.tm = mutate(mum.tm,m);
    baby2.tm = mutate(dad.tm,m);
    new_cm_vec.push_back(baby1);
    new_cm_vec.push_back(baby2);
  }
  cm_vec = new_cm_vec;
}

int Ga_engine::bestfitness() {
  int best = infinit;
  for (int i = 0; i < pop_size; i++) {
    if (cm_vec[i].fitness < best) {
      best = cm_vec[i].fitness;
      best_cm = cm_vec[i];
    }
  }
  return best;
}

Chromosome Ga_engine::ga(int n, int m, int num) {
  init(n, m, num);
  for (int i = 0; i < generation; i++) {
    for (int j = 0; j < pop_size; j++) {
      int allo_cnt = 0, bad_allo = 0;
      Chromosome temp_cm = cm_vec[j];
      Allocated* temp_allo = allo_with_cm(temp_cm, allo_cnt, bad_allo);
      cm_vec[i].fitness = evaluate(temp_allo, allo_cnt, bad_allo);
    }
    epoch();
    if (bestfitness() < 200)break;
  }
  return best_cm;
}

void greedy(Allocated*& allo, int cnt, int& allo_cnt,int& bad_allo) {
  for (int s = 0; s < cnt; s++) {
    int flag = 0;
    for (int tempy = ship[s]->arv_tm; tempy + ship[s]->svc_tm - 1 < m; tempy++) {
      for (int i = 0; i + ship[s]->berth_occupy - 1 < n; i++) {
        int lx = i; int ly = tempy;
        int rx = lx + ship[s]->berth_occupy - 1;
        int ry = ly + ship[s]->svc_tm - 1;
        int overlap = 0;
        if (lx == 6) {
          int x = 0;
        }
        judge(allo, allo_cnt, lx, ly, rx, ry, overlap);
        if (!overlap) {
          allo[allo_cnt].leftx = lx;
          allo[allo_cnt].lefty = ly;
          allo[allo_cnt].rightx = rx;
          allo[allo_cnt].righty = ry;
          allo[allo_cnt].id = ship[s]->id;
          allo_cnt++;
          flag = 1;
          break;
        }
      }
      if (flag)break;
    }//for tempy
    if (!flag) {
      bad_allo++;
    }
  }
}

int main() {
  ifstream bafile("C:\\Users\\Geng David\\Documents\\berth_list.txt");
  ofstream outfile("C:\\Users\\Geng David\\Documents\\result.txt", ofstream::app);
  string s;
  if (!bafile.is_open()) {
    cout << "erro open file!" << endl;
  }
  while (getline(bafile, s)) {
    int com_flag = 0;
    int n_cnt = 0, m_cnt = 0;
    n = 0;  m = 0;
    for (int i = 0; i < (int)s.find_first_of(';'); i++) {
      if (is_num(s[i])) {
        if (!com_flag) {
          n *= (int)pow(10, n_cnt);
          n += s[i] - '0';
          n_cnt++;
        }
        else {
          m *= (int)pow(10, m_cnt);
          m += s[i] - '0';
          m_cnt++;
        }
      }
      else if (s[i] == ',')com_flag = 1;
    }

    int cnt = 0;
    int num0fship = (s.length() - s.find_first_of(';')) / 6;
    ship = new Ship*[num0fship];
    int buffer[3];
    for (int i = s.find_first_of(';'); i < (int)s.length() - 1; i = s.find_first_of(';',i + 1)) {
      int bf_cnt = 0;
      for (int j = i + 1; bf_cnt < 3; j++) {
        if (s[j] == ',')continue;
        buffer[bf_cnt++] = s[j] - '0';
      }
      ship[cnt] = new Ship(cnt,buffer[0],buffer[1],buffer[2]);
      cnt++;
    }

    Allocated* allo = new Allocated[cnt];
    int allo_cnt = 0, bad_allo = 0;
    //greedy(allo, cnt, allo_cnt, bad_allo);
    srand((unsigned)time(NULL));
    Ga_engine gae(1001,1001);
    Chromosome result = gae.ga(n,m,num0fship);
    allo = gae.allo_with_cm(result, allo_cnt, bad_allo);
    int grade = evaluate(allo, allo_cnt, bad_allo);


    /*writing result to files*/
    int out_cnt = 0;
    for (int i = 0; i < cnt; i++) {
      if (allo[out_cnt].id == i) {
        string s = "";
        stringstream ss;
        ss << allo[out_cnt].leftx << ',' << allo[out_cnt].lefty << ';';
        ss >> s;
        outfile << s;     
        out_cnt++;
      }
      else {
        string s = "-1,-1;";
        outfile << s;
      }
    }
    outfile << endl;

    for (int i = 0; i < num0fship; i++) {
      delete[] ship[i];
    }
    delete[] ship;
    ship = nullptr;

  }
  bafile.close();
  outfile.close();
  return 0;
}

