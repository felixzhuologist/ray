#ifndef BVHH
#define BVHH

#include "hitable.h"

class bvh_node : public hitable {
public:
  bvh_node() {}
  bvh_node(hitable **l, int n);
  virtual bool hit(const ray &r, float t_min, float t_max, hit_record &rec) const;
  virtual bool bounding_box(aabb &box) const;

  hitable *left;
  hitable *right;
  aabb box;
};

// TODO: there must be a way to avoid this in c++...
int box_x_compare (const void *a, const void *b) {
  aabb box_left, box_right;
  hitable *ah = *(hitable**)a;
  hitable *bh = *(hitable**)b;
  if(!ah->bounding_box(box_left) || !bh->bounding_box(box_right))
    std::cerr << "no bounding box in bvh_node constructor\n";
  if (box_left.min().x() - box_right.min().x() < 0.0  )
    return -1;
  else
    return 1;
}

int box_y_compare (const void *a, const void *b) {
  aabb box_left, box_right;
  hitable *ah = *(hitable**)a;
  hitable *bh = *(hitable**)b;
  if(!ah->bounding_box(box_left) || !bh->bounding_box(box_right))
    std::cerr << "no bounding box in bvh_node constructor\n";
  if (box_left.min().y() - box_right.min().y() < 0.0  )
    return -1;
  else
    return 1;
}

int box_z_compare (const void *a, const void *b) {
  aabb box_left, box_right;
  hitable *ah = *(hitable**)a;
  hitable *bh = *(hitable**)b;
  if(!ah->bounding_box(box_left) || !bh->bounding_box(box_right))
    std::cerr << "no bounding box in bvh_node constructor\n";
  if (box_left.min().z() - box_right.min().z() < 0.0  )
    return -1;
  else
    return 1;
}

bvh_node::bvh_node(hitable **l, int n) {
  // randomly pick an axis to bisect
  int axis = int(3 * dis(gen));
  if (axis == 0) {
    qsort(l, n, sizeof(hitable *), box_x_compare);
  } else if (axis == 1) {
    qsort(l, n, sizeof(hitable *), box_y_compare);
  } else {
    qsort(l, n, sizeof(hitable *), box_z_compare);
  }

  if (n == 1) {
    left = right = l[0];
  } else if (n == 2) {
    left = l[0];
    right = l[1];
  } else {
    left = new bvh_node(l, n/2);
    right = new bvh_node(l + n/2, n - n/2);
  }

  aabb box_left, box_right;
  if(!left->bounding_box(box_left) || !right->bounding_box(box_right))
    std::cerr << "no bounding box in bvh_node constructor\n"; 
  box = surrounding_box(box_left, box_right);
}

bool bvh_node::hit(const ray &r, float t_min, float t_max, hit_record &rec) const {
  if (!box.hit(r, t_min, t_max)) {
    return false;
  }
  hit_record left_rec, right_rec;
  bool hit_left = left->hit(r, t_min, t_max, left_rec);
  bool hit_right = right->hit(r, t_min, t_max, right_rec);
  if (hit_left && hit_right) {
    rec = left_rec.t < right_rec.t ? left_rec : right_rec;
    return true;
  } else if (hit_left) {
    rec = left_rec;
    return true;
  } else if (hit_right) {
    rec = right_rec;
    return true;
  }
  return false;
}

bool bvh_node::bounding_box(aabb &b) const {
  b = box;
  return true;
}

#endif
