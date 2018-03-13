#include <iostream>
#include "float.h"
#include "hitable_list.h"
#include "sphere.h"

vec3 SPHERE_CENTER = vec3(0, 0, -1);
float SPHERE_RADIUS = 0.5;

// Return the background color for a ray, which is a linear interpolation
// of the its y direction (white at 0 and blue at 1)
vec3 background_color(const ray &r) {
  vec3 unit_direction = unit_vector(r.direction());
  // map y direction from [-1, 1] to [0, 1]
  float t = 0.5*(unit_direction.y() + 1.0);
  return (1.0-t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 1.0);
}

vec3 color(const ray &r, hitable *world) {
  hit_record rec;
  // min t is 0 since we don't care about things behind us
  if (world->hit(r, 0.0, MAXFLOAT, rec)) {
    // convert normal to a color by getting unit vector and mapping to 0, 1
    return 0.5 * vec3(rec.normal.x() + 1, rec.normal.y() + 1, rec.normal.z() + 1);
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

  hitable *objects[2];
  objects[0] = new sphere(vec3(0, 0, -1), 0.5);
  objects[1] = new sphere(vec3(0, -100.5, -1), 100);
  hitable *world = new hitable_list(objects, 2);
  for (int j = ny - 1; j >= 0; j--) {
    for (int i = 0; i < nx; i++) {
      // u is x vector of current pixel and v is the y vector
      vec3 u = (float(i) / float(nx)) * horizontal;
      vec3 v = (float(j) / float(ny)) * vertical;
      ray r(origin, lower_left_corner + u + v);
      vec3 pixel = 255.99 * color(r, world); // map [0, 1) to [0, 256)
      std::cout << pixel << "\n";
    }
  }
}
