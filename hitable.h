#ifndef HITABLEH
#define HITABLEH

#include "aabb.h"

class material;

// information about where a ray hits an object/"hitable"
struct hit_record {
  float t;
  vec3 p;
  vec3 normal;
  material *mat_ptr;
};

// abstract class of an object that can be hit by a ray
// implements hit() which determines whether this object is hit by the
// input ray
class hitable {
public:
  // return true and fill in values of the hit_record if ray hits the hitable
  // takes in t_min and t_max which is restricts the values of t that are considered
  virtual bool hit(const ray &r, float t_min, float t_max, hit_record &rec) const = 0;
  virtual bool bounding_box(aabb &box) const = 0;
};

#endif
