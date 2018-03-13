#ifndef SPHEREH
#define SPHEREH

#include "hitable.h"

class sphere: public hitable {
public:
  sphere() {}
  sphere(vec3 c, float r) : center(c), radius(r) {};
  virtual bool hit(const ray &r, float t_min, float t_max, hit_record &rec) const;

  vec3 center;
  float radius;
};

// a point on the ray p(t) is also on the sphere iff:
// (p(t) - C) · (p(t) - C) = r^2
// we can therefore solve for the roots of the above quadratic equation:
// (A + tB - C) · (A + tB - C) = r^2
// t^2|B|^2 + 2t(B · (A-C)) + |A-C|^2 - r^2 = 0
// (which gives us something of the form: at^2 + bt + c = 0)
bool sphere::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
    vec3 a_minus_c = r.origin() - center;
    float a = dot(r.direction(), r.direction());
    // the *2 in the b term is cancelled out later
    float b = dot(a_minus_c, r.direction());
    float c = dot(a_minus_c, a_minus_c) - radius*radius;
    float discriminant = b*b - a*c;
    if (discriminant > 0) {
        float soln = (-b - sqrt(discriminant)) / a;
        if (soln <= t_min || soln >= t_max) {
          soln = (-b + sqrt(discriminant)) / a; // try other soln
        }
        if (soln < t_max && soln > t_min) {
            rec.t = soln;
            rec.p = r.point_at_parameter(rec.t);
            rec.normal = (rec.p - center) / radius;
            return true;
        }
    }
    return false;
}

#endif
