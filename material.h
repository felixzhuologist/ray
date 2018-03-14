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

// we know that the direction of the reflected ray is v + 2b. We know also know the
// direction of b already (it is parallel to n) and we can get its length from v \dot n
//      n     
//  v \ | / ret val
//     \|/ |
// ____________
//     v \ |
//        \| b    
vec3 reflect(const vec3 &v, const vec3 &n) {
  vec3 b = dot(v, n) * n;
  return v - 2*b;
}

bool refract(const vec3 &v, const vec3 &n, float n_ratio, vec3 &refracted) {
  vec3 uv = unit_vector(v);
  float dt = dot(uv, n);
  float discriminant = 1 - n_ratio*(1 - dt*dt);
  if (discriminant > 0) {
    refracted = n_ratio*(uv - n*dt) - n*sqrt(discriminant);
  }
  // total internal reflection
  return false;
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

class metal : public material {
public:
  metal(const vec3 a, float f): albedo(a) {if (fuzz < 1 && fuzz >= 0) fuzz = f; else fuzz = 1; }
  virtual bool scatter(const ray r, const hit_record rec, vec3 &attenuation, ray &scattered) const {
    vec3 reflected = reflect(unit_vector(r.direction()), rec.normal);
    // fuzz reflection by picking point on sphere of radius fuzz centered around reflection
    vec3 fuzzed = reflected + fuzz*random_in_unit_sphere();
    scattered = ray(rec.p, fuzzed);
    attenuation = albedo;
    return (dot(scattered.direction(), rec.normal) > 0);
  }

  vec3 albedo;
  float fuzz;
};

class dielectric : public material {
public:
  dielectric(float ri) : refract_idx(ri) {}
  virtual bool scatter(const ray r, const hit_record rec, vec3 &attenuation, ray &scattered) const {
    vec3 outward_normal;
    vec3 reflected = reflect(r.direction(), rec.normal);
    float n_ratio;
    attenuation = vec3(1.0, 1.0, 1.0);
    vec3 refracted;
    if (dot(r.direction(), rec.normal) > 0) {
      outward_normal = -rec.normal;
      n_ratio = refract_idx;
    } else {
      outward_normal = rec.normal;
      n_ratio = 1.0 /refract_idx;
    }

    if (refract(r.direction(), outward_normal, n_ratio, refracted)) {
      scattered = ray(rec.p, refracted);
    } else {
      scattered = ray(rec.p, reflected);
    }
    return true;
  }

  float refract_idx;
};

#endif
