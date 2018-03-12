#include <iostream>
#include "vec3.h"

int main() {
  int nx = 200;
  int ny = 100;
  std::cout << "P3\n" << nx << " " << ny << "\n255\n";
  for (int j = ny - 1; j >= 0; j--) {
    for (int i = 0; i < nx; i++) {
      vec3 pixel(float(i) / float(nx), float(j) / float(ny), 0.2);
      // map [0, 1) to [0, 256)
      std::cout << 255.99 * pixel << "\n";
    }
  }
}
