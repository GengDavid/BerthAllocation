#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <cstring>
#include <algorithm>
#include <vector>
#include <time.h>
#include "Ship.h"
#include "Alloc_list.h"
#include "Ba_func.h"
#include "Ga_engine.h"

using std::ifstream;
using std::ofstream;
using std::string;
using std::stringstream;
using std::vector;
using std::cin;
using std::cout;
using std::endl;
using std::sort;

int n, m;

int main() {
  int scase = 1;
  ifstream bafile("list1.txt");
  ofstream outfile("result.txt", ofstream::app);
  string s;
  int select = 0;
  while (select != 1 && select != 2) {
    cout << "If you want to run with greedy, type in 1" << endl;
    cout << "Or if you want to run Ga, type in 2" << endl;
    cin >> select;
  }
  if (!bafile.is_open()) {
    cout << "erro open file!" << endl;
    exit(0);
  }
  if (select == 1)outfile << "greedy:" << endl;
  else outfile << "GA:" << endl;
  /*run all cases in file*/
  while (getline(bafile, s)) {
    stringstream case_ss;
    case_ss << "case:" << scase;
    string case_s;
    case_ss >> case_s;
    outfile << s;

    /*get the area */
    int com_flag = 0;
    int n_cnt = 0, m_cnt = 0;
    n = 0;  m = 0;
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
    
    /*generate ships*/
    printf("case:%d\n", scase);
    int cnt = 0;
    int numOfship = (int)(s.length() - s.find_first_of(';')) / 6;
    vector<Ship> ship;
    int buffer[3];
    for (size_t i = s.find_first_of(';'); i < s.length() - 1; i = s.find_first_of(';',i + 1)) {
      int bf_cnt = 0;
      string ships = "";
      stringstream ss;
      for (size_t j = i + 1; bf_cnt < 3; j++) {
        if (s[j] == ',' || s[j] == ';') {
          ss << ships.c_str();
          ss >> buffer[bf_cnt];
          ships = "";
          ss.clear();
          bf_cnt++;
        }
        else {
          ships += s[j];
        }
      }
      ship.push_back(Ship(cnt,buffer[0],buffer[1],buffer[2]));
      cnt++;
    }

    /*using greedy or Ga to run*/
    double tm_vec;
    time_t beg, ed;
    Alloc_list alloc_list;
    if (select==1) {
      beg = clock();
      alloc_list = greedy(ship);
      ed = clock();
    }
    else if(select==2) {
      srand((unsigned int)time(0));
      Ga_engine ge;
      int s_size = ship.size();
      int coef = 10;
      ge.init(coef*s_size, coef*s_size, 0.1, ship);
      beg = clock();
      alloc_list = ge.begin();
      ed = clock();
    }
    tm_vec = double(ed - beg)/CLOCKS_PER_SEC;
    Loss ls = evaluate2(alloc_list, ship);
    sort(alloc_list.allocated.begin(), alloc_list.allocated.end(), allo_cmp);

    /*writing result to files*/
    int out_cnt = 0;
    for (int i = 0; i < ship.size(); i++) {
      if (out_cnt<alloc_list.allocated.size()&&alloc_list.allocated[out_cnt].id == i) {
        string s = "";
        stringstream ss;
        ss << alloc_list.allocated[out_cnt].leftx << ',' << alloc_list.allocated[out_cnt].lefty << ';';
        ss >> s;
        outfile << s;
        out_cnt++;
      }
      else {
        string s = "-1,-1;";
        outfile << s;
      }
    }
    cout << "penalty: " << ls.pen << " un_alloc: " << ls.bad_allo << " total_waiting: " << ls.ttwt << " last_departure_time: " << ls.lstdpt << endl;
    stringstream ss;
    string outs;
    ss << "time:" << tm_vec;
    ss >> outs;
    outs += 's';
    outfile << outs;
    outfile << endl;
    scase++;
  }
  outfile << endl;
  bafile.close();
  outfile.close();
  return 0;
}