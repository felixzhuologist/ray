#include <random>
#include <iostream>

#include "bvh.h"
#include "camera.h"
#include "float.h"
#include "material.h"
#include "sphere.h"
#include "texture.h"

// Return the background color for a ray, which is a linear interpolation
// of the its y direction (white at 0 and blue at 1)
vec3 background_color(const ray &r) {
  vec3 unit_direction = unit_vector(r.direction());
  // map y direction from [-1, 1] to [0, 1]
  float t = 0.5*(unit_direction.y() + 1.0);
  return (1.0-t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 1.0);
}

 
vec3 color(const ray &r, hitable *world, int depth) {
  hit_record rec;
  // min t is 0 since we don't care about things behind us, and ignore hits
  // very close to 0 to avoid shadow acne
  if (world->hit(r, 0.001, MAXFLOAT, rec)) {
    ray scattered;
    vec3 attenuation;
    if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
      return attenuation * color(scattered, world, depth + 1);
    } else {
      return vec3(0.0, 0.0, 0.0);
    }
  }
  return background_color(r);
}

hitable *random_scene() {
  int n = 500;
  hitable **list = new hitable*[n+1];
  list[0] =  new sphere(vec3(0, -1000, 0), 1000, new lambertian(new constant_texture(vec3(0.5, 0.5, 0.5))));
  int i = 1;
  for (int a = -11; a < 11; a++) {
    for (int b = -11; b < 11; b++) {
      float choose_mat = dis(gen);
      vec3 center(a + 0.9*dis(gen), 0.2, b + 0.9*dis(gen)); 
      if ((center - vec3(4, 0.2, 0)).length() > 0.9) { 
        if (choose_mat < 0.8) {  // diffuse
          list[i++] = new sphere(center, 0.2, new lambertian(new constant_texture(vec3(dis(gen)*dis(gen), dis(gen)*dis(gen), dis(gen)*dis(gen)))));
        }
        else if (choose_mat < 0.95) { // metal
          list[i++] = new sphere(center, 0.2, new metal(vec3(0.5*(1 + dis(gen)), 0.5*(1 + dis(gen)), 0.5*(1 + dis(gen))),  0.5*dis(gen)));
        }
        else { // glass
          list[i++] = new sphere(center, 0.2, new dielectric(1.5));
        }
      }
    }
  }
  list[i++] = new sphere(vec3(0, 1, 0), 1.0, new dielectric(1.5));
  list[i++] = new sphere(vec3(-4, 1, 0), 1.0, new lambertian(new constant_texture(vec3(0.4, 0.2, 0.1))));
  list[i++] = new sphere(vec3(4, 1, 0), 1.0, new metal(vec3(0.7, 0.6, 0.5), 0.0));
  return new bvh_node(list, i);
}

hitable *small_scene() {
  hitable *objects[4];
  texture *check = new checker_texture(
    new constant_texture(vec3(0.2, 0.3, 0.1)),
    new constant_texture(vec3(0.9, 0.9, 0.9)));
  objects[0] = new sphere(vec3(0, 0, -1), 0.5, new lambertian(new constant_texture(vec3(0.8, 0.3, 0.3))));
  objects[1] = new sphere(vec3(0, -100.5, -1), 100, new lambertian(check));
  objects[2] = new sphere(vec3(1, 0, -1), 0.5, new metal(vec3(0.8, 0.6, 0.2), 0.3));
  objects[3] = new sphere(vec3(-1, 0, -1), 0.5, new dielectric(1.5));
  return new bvh_node(objects, 4);
}

int main() {
  int nx = 200;
  int ny = 100;
  int ns = 100;
  std::cout << "P3\n" << nx << " " << ny << "\n255\n";

  camera cam(vec3(0, 0, 1), vec3(0, 0, -1), vec3(0, 1, 0.8), 90, float(nx)/float(ny));
  // camera cam(vec3(13, 2, 3), vec3(0, 0, 0), vec3(0, 1, 0), 20, float(nx)/float(ny));
  hitable *world = small_scene();

  for (int j = ny - 1; j >= 0; j--) {
    for (int i = 0; i < nx; i++) {
      vec3 pixel(0, 0, 0);
      for (int s = 0; s < ns; s++) {
        float u = float(i + dis(gen)) / float(nx);
        float v = float(j + dis(gen)) / float(ny);
        ray r = cam.get_ray(u, v);
        pixel += color(r, world, 0);
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
