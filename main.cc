#include <iostream>
#include "ray.h"

vec3 SPHERE_CENTER = vec3(0, 0, -1);
float SPHERE_RADIUS = 0.5;

// does ray r hit the sphere at given center C and radius r?
//
// a point on the ray p(t) is also on the sphere iff:
// (p(t) - C) · (p(t) - C) = r^2
// we can therefore solve for the roots of the above quadratic equation:
// (A + tB - C) · (A + tB - C) = r^2
// t^2|B|^2 + 2t(B · (A-C)) + |A-C|^2 - r^2 = 0
// (which gives us something of the form: at^2 + bt + c = 0)
bool hit_sphere(const vec3 &center, float radius, const ray &r) {
  vec3 a_minus_c = r.origin() - center;
  float a = dot(r.direction(), r.direction());
  float b = 2 * dot(r.direction(), a_minus_c);
  float c = dot(a_minus_c, a_minus_c) - radius*radius;
  float discriminant = b*b - 4*a*c;
  return (discriminant > 0);
}

// Return the background color for a ray, which is a linear interpolation
// of the its y direction (white at 0 and blue at 1)
vec3 background_color(const ray &r) {
  vec3 unit_direction = unit_vector(r.direction());
  // map y direction from [-1, 1] to [0, 1]
  float t = 0.5*(unit_direction.y() + 1.0);
  return (1.0-t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 1.0);
}

vec3 color(const ray &r) {
  if (hit_sphere(SPHERE_CENTER, SPHERE_RADIUS, r)) {
    return vec3(1, 0, 0);
  }
  return background_color(r);
}

int main() {
  int nx = 200;
  int ny = 100;
  std::cout << "P3\n" << nx << " " << ny << "\n255\n";
  vec3 lower_left_corner(-2.0, -1.0, -1.0);
  vec3 horizontal(4.0, 0.0, 0.0);
  vec3 vertical(0.0, 2.0, 0.0);
  vec3 origin(0.0, 0.0, 0.0);
  for (int j = ny - 1; j >= 0; j--) {
    for (int i = 0; i < nx; i++) {
      // u is x vector of current pixel and v is the y vector
      vec3 u = (float(i) / float(nx)) * horizontal;
      vec3 v = (float(j) / float(ny)) * vertical;
      ray r(origin, lower_left_corner + u + v);
      vec3 pixel = 255.99 * color(r); // map [0, 1) to [0, 256)
      std::cout << pixel << "\n";
    }
  }
}
