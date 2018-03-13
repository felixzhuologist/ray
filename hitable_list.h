#ifndef HITABLELISTH
#define HITABLELISTH

#include "hitable.h"

class hitable_list: public hitable {
public:
  hitable_list() {}
  hitable_list(hitable **l, int n) { list = l, list_size = n; }
  virtual bool hit(const ray &r, float t_min, float t_max, hit_record &rec) const;

  hitable **list;
  int list_size;
};

// use each hitable's hit() method to determine which hitable (if any) was closest
bool hitable_list::hit(const ray &r, float t_min, float t_max, hit_record &rec) const {
  hit_record current_rec;
  bool hit_anything = false;
  double closest_so_far = t_max;
  for (int i = 0; i < list_size; i++) {
    if (list[i]->hit(r, t_min, closest_so_far, current_rec)) {
      hit_anything = true;
      closest_so_far = current_rec.t;
      rec = current_rec;
    }
  }
  return hit_anything;
}

#endif
