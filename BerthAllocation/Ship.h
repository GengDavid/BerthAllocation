#pragma once
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
