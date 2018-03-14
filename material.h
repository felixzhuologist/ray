#ifndef MATERIALH
#define MATERIALH

#include "hitable.h"

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<> dis(0.0, 1.0);

// return a random point in unit sphere
vec3 random_in_unit_sphere() {
  vec3 p;
  do {
    // random point in unit cube (each point is in -1, 1)
    p = 2*vec3(dis(gen), dis(gen), dis(gen)) - vec3(1.0, 1.0, 1.0);
  } while (p.squared_length() >= 1.0);
  return p;
}

class material {
public:
  virtual bool scatter(const ray r, const hit_record rec, vec3 &attenuation, ray &scattered) const = 0;
};

class lambertian : public material {
public:
  lambertian(const vec3 a): albedo(a) {}
  virtual bool scatter(const ray r, const hit_record rec, vec3 &attenuation, ray &scattered) const {
    // generate a random target for the diffused ray by picking random point on
    // the unit sphere tangent to the hit point rec.p - the color for the current
    // ray will be that of the target, but with 50% of the energy absorbed
    vec3 target = rec.p + rec.normal + random_in_unit_sphere();
    scattered = ray(rec.p, target - rec.p);
    attenuation = albedo;
    return true;
  }

  vec3 albedo;
};

#endif
