#include <random>
#include <iostream>

#include "camera.h"
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
  int ns = 100;
  std::cout << "P3\n" << nx << " " << ny << "\n255\n";

  camera cam;
  hitable *objects[2];
  objects[0] = new sphere(vec3(0, 0, -1), 0.5);
  objects[1] = new sphere(vec3(0, -100.5, -1), 100);
  hitable *world = new hitable_list(objects, 2);

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> dis(0.0, 1.0);

  for (int j = ny - 1; j >= 0; j--) {
    for (int i = 0; i < nx; i++) {
      vec3 pixel(0, 0, 0);
      for (int s = 0; s < ns; s++) {
        float u = float(i + dis(gen)) / float(nx);
        float v = float(j + dis(gen)) / float(ny);
        ray r = cam.get_ray(u, v);
        pixel += color(r, world);
      }
      // average color over each sample and map [0, 1) to [0, 256)
      pixel = (pixel * 255.99) / float(ns);
      std::cout << pixel << "\n";
    }
  }
}
