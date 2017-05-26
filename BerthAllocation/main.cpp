#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <cstring>

using std::ifstream;
using std::ofstream;
using std::string;
using std::stringstream;
using std::cout;
using std::endl;

class Ship {
public:
  Ship() {}
  Ship(int id, int arv, int svc, int berth) 
    :id(id), arv_tm(arv), svc_tm(svc), berth_occupy(berth) {}

  int id;
  int arv_tm;
  int svc_tm;
  int berth_occupy;
};

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

int main() {
  ifstream bafile("C:\\Users\\Geng David\\Documents\\berth_list.txt");
  ofstream outfile("C:\\Users\\Geng David\\Documents\\result.txt", ofstream::app);
  string s;
  if (!bafile.is_open()) {
    cout << "erro open file!" << endl;
  }
  while (getline(bafile, s)) {
    int com_flag = 0;
    int n = 0, m = 0;
    int n_cnt = 0, m_cnt = 0;
    for (int i = 0; i < s.find_first_of(';'); i++) {
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
    Ship **ship = new Ship*[num0fship];
    int buffer[3];
    for (int i = s.find_first_of(';'); i < s.length() - 1; i = s.find_first_of(';',i + 1)) {
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
          judge(allo, allo_cnt,lx,ly,rx,ry,overlap);
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
    int ttwt = 0, last_departure_tm = -1;
    for (int i = 0; i < allo_cnt; i++) {
      ttwt += allo[i].lefty - ship[allo[i].id]->arv_tm;
      if (allo[i].righty + 1 > last_departure_tm)last_departure_tm = allo[i].righty + 1;
    }
    outfile << endl;
    cout << bad_allo << " " << ttwt << " " << last_departure_tm << endl;
  }
  bafile.close();
  outfile.close();
  return 0;
}