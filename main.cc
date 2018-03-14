#include <random>
#include <iostream>

#include "camera.h"
#include "float.h"
#include "hitable_list.h"
#include "material.h"
#include "sphere.h"

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
  // min t is 0 since we don't care about things behind us, and ignore hits
  // very close to 0 to avoid shadow acne
  if (world->hit(r, 0.001, MAXFLOAT, rec)) {
    ray scattered;
    vec3 attenuation;
    if (rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
      return attenuation * color(scattered, world);
    }
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
  objects[0] = new sphere(vec3(0, 0, -1), 0.5, new lambertian(vec3(0.5, 0.5, 0.5)));
  objects[1] = new sphere(vec3(0, -100.5, -1), 100, new lambertian(vec3(0.5, 0.5, 0.5)));
  hitable *world = new hitable_list(objects, 2);

  for (int j = ny - 1; j >= 0; j--) {
    for (int i = 0; i < nx; i++) {
      vec3 pixel(0, 0, 0);
      for (int s = 0; s < ns; s++) {
        float u = float(i + dis(gen)) / float(nx);
        float v = float(j + dis(gen)) / float(ny);
        ray r = cam.get_ray(u, v);
        pixel += color(r, world);
      }

      // average color over each sample, apply gamma correction of gamma=2 to 
      // make objects lighter, then map rgb values from 0, 1 (real) to 0, 256 (integer)
      // for ppm format
      pixel /= float(ns);
      pixel = vec3(sqrt(pixel.r()), sqrt(pixel.g()), sqrt(pixel.b()));
      pixel *= 255.99;
      std::cout << pixel << "\n";
    }
  }
}
