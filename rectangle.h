#ifndef RECTANGLEH
#define RECTANGLEH

#include "hitable.h"
#include "material.h"

class xy_rect: public hitable {
public:
  xy_rect() {}
  xy_rect(float _x0, float _x1, float _y0, float _y1, float _k, material *mat) :
    mp(mat), x0(_x0), x1(_x1), y0(_y0), y1(_y1), k(_k) {};
  virtual bool hit(const ray &r, float t_min, float t_max, hit_record &rec) const;
  virtual bool bounding_box(aabb &box) const;

  material *mp;
  float x0, x1, y0, y1, k;
};

bool xy_rect::hit(const ray &r, float t_min, float t_max, hit_record &rec) const {
  float t = (k - r.origin().x()) / r.direction().z(); // get t where ray hits rectangle
  if (t < t_min || t > t_max)
    return false;

  float x = r.origin().x() + t*r.direction().x(); // get x, y where ray hits rectangle
  float y = r.origin().y() + t*r.direction().y();
  if (x < x0 || x > x1 || y < y0 || y > y1)
    return false;

  rec.t = t;
  rec.mat_ptr = mp;
  rec.p = r.point_at_parameter(t);
  rec.normal = vec3(0, 0, 1);
  return true;
}

bool xy_rect::bounding_box(aabb &box) const {
  // add z thickness of 0.0002 to create bounding box
  box = aabb(vec3(x0, y0, k - 0.0001), vec3(x1, y1, k + 0.0001));
  return true;
}

#endif
