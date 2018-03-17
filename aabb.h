#ifndef AABBH
#define AABBH

#include <algorithm>
#include "ray.h"

class aabb {
public:
  aabb() {}
  // min/max corners defining the bounding box
  aabb(vec3 a, vec3 b) { _min = a; _max = b; }

  vec3 min() const { return _min; }
  vec3 max() const { return _max; }

  bool hit(const ray &r, float tmin, float tmax) const {
    for (int dim = 0; dim < 3; dim++) {
      // multiply by inverse so that we do 1 divide + 2 mult vs 2 divides as
      // multiplication is faster on most hardware: https://goo.gl/qLmQmL
      float inv_dir = 1.0f / r.direction()[dim];
      // solve for points t0/t1 where the ray hits the box boundaries for this dimension
      float t0 = (min()[dim] - r.origin()[dim]) * inv_dir;
      float t1 = (max()[dim] - r.origin()[dim]) * inv_dir;
      if (inv_dir < 0.0f)
        std::swap(t0, t1); // need intervals to be ordered when checking for intersection
      tmin = t0 > tmin ? t0 : tmin;
      tmax = t1 < tmax ? t1 : tmax;
      if (tmax <= tmin)
        return false;
    }
    // the ray goes through the bounding box if all t0/t1 intervals intersect
    return true;
  }

  vec3 _min;
  vec3 _max;
};

aabb surrounding_box(const aabb box0, const aabb box1) {
  vec3 min_corner(
    fmin(box0.min().x(), box1.min().x()),
    fmin(box0.min().y(), box1.min().y()),
    fmin(box0.min().z(), box1.min().z()));
  vec3 max_corner(
    fmax(box0.max().x(), box1.max().x()),
    fmax(box0.max().y(), box1.max().y()),
    fmax(box0.max().z(), box1.max().z()));
  return aabb(min_corner, max_corner);
}

#endif
