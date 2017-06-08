/*
* Including functions about allocation
*/
#include "Ba_func.h"
extern int n;
extern int m;

bool is_num(char c) {
  return c >= '0'&&c <= '9';
}

/*
* judge whether the ship is overlape with the allocated ships
*/
bool judge(const vector<Allocated>& allo, int lx, int ly, int rx, int ry) {
  int overlap = 0;
  int **g = new int*[n];
  for (int i = 0; i < n; i++) {
    g[i] = new int[m];
  }
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      g[i][j] = 0;
    }
  }

  for (int s = 0; s < allo.size(); s++) {
    for (int i = allo[s].leftx; i <= allo[s].rightx; i++) {
      for (int j = allo[s].lefty; j <= allo[s].righty; j++) {
        g[i][j] = 1;
      }
    }
  }
  for (int i = lx; i <= rx; i++) {
    for (int j = ly; j <= ry; j++) {
      if (g[i][j]) {
        overlap = 1;
        break;
      }
    }
    if (overlap)break;
  }
  /*for (int j = 0; j < allo.size(); j++) {
    if ((rx >= allo[j].leftx&&rx <= allo[j].rightx&&ry >= allo[j].lefty&&ry <= allo[j].righty) ||
      (rx >= allo[j].leftx&&rx <= allo[j].rightx&&ly >= allo[j].lefty&&ly <= allo[j].righty) ||
      (lx >= allo[j].leftx&&lx <= allo[j].rightx&&ry >= allo[j].lefty&&ry <= allo[j].righty) ||
      (lx >= allo[j].leftx&&lx <= allo[j].rightx&&ly >= allo[j].lefty&&ly <= allo[j].righty) ||
      (allo[j].rightx >= lx&&allo[j].rightx <= rx&&allo[j].righty >= ly&&allo[j].righty <= ry) ||
      (allo[j].rightx >= lx&&allo[j].rightx <= rx&&allo[j].lefty >= ly &&allo[j].lefty <= ry) ||
      (allo[j].leftx >= lx&&allo[j].leftx <= rx&&allo[j].righty >= ly&&allo[j].righty <= ry) ||
      (allo[j].leftx >= lx&&allo[j].leftx <= rx&&allo[j].lefty >= ly&&allo[j].lefty <= ry) ||
      ((lx == allo[j].leftx) && (rx == allo[j].rightx) && (ly == allo[j].lefty) && (ry = allo[j].righty)))
      overlap = 1;
    }*/
  for (int i = 0; i < n; i++) {
    delete[] g[i];
  }
  delete[] g;
  return overlap;
}

/*
* evaluate the penalty of a scheme of allocation
* return val: bad_allo * 100 + ttwt * 2 + last_departure_tm * 1
*/
int evaluate(const Alloc_list& alloc_list, const vector<Ship>& ship) {
  vector<Allocated> allo = alloc_list.allocated;
  int ttwt = 0, last_departure_tm = -1;
  for (int i = 0; i < allo.size(); i++) {
    ttwt += allo[i].lefty - ship[allo[i].id].arv_tm;
    if (allo[i].righty + 1 > last_departure_tm)last_departure_tm = allo[i].righty + 1;
  }
  return alloc_list.bad_allo * 100 + ttwt * 2 + last_departure_tm * 1;
}

/*
* evaluation function 2 of a scheme of allocation
* return val: penalty and details(with Loss struct)
*/
Loss evaluate2(const Alloc_list& alloc_list, const vector<Ship>& ship)
{
  vector<Allocated> allo = alloc_list.allocated;
  int ttwt = 0, last_departure_tm = -1;
  for (int i = 0; i < allo.size(); i++) {
    ttwt += allo[i].lefty - ship[allo[i].id].arv_tm;
    if (allo[i].righty + 1 > last_departure_tm)last_departure_tm = allo[i].righty + 1;
  }
  int pen = alloc_list.bad_allo * 100 + ttwt * 2 + last_departure_tm * 1;
  return Loss(alloc_list.bad_allo, ttwt, last_departure_tm, pen);
}

/*
* greedy function
* @ship, ships that will be placed in order
* return an allocation scheme
*/
Alloc_list greedy(const vector<Ship>& ship) {
  int bad_allo = 0;
  vector<Allocated> allo;
  for (int s = 0; s < ship.size(); s++) {
    int flag = 0;
    for (int tempy = ship[s].arv_tm; tempy + ship[s].svc_tm  <= m; tempy++) {
      for (int i = 0; i + ship[s].berth_occupy <= n; i++) {
        int lx = i; int ly = tempy;
        int rx = lx + ship[s].berth_occupy - 1;
        int ry = ly + ship[s].svc_tm - 1;
        int overlap = 0;
        overlap = judge(allo, lx, ly, rx, ry);
        if (!overlap) {
          allo.push_back(Allocated(lx, ly, rx, ry, ship[s].id));
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
  return Alloc_list(allo, bad_allo);
}

bool allo_cmp(const Allocated& a1, const Allocated& a2) {
  return a1.id < a2.id;
}
