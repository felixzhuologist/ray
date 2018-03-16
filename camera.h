#ifndef CAMERAH
#define CAMERAH

#include "ray.h"

class camera {
public:

  /*
   * lookfrom: origin point of the camera
   * lookat: destination point that the camera is looking at
   * vup: vup is mapped onto camera plane to determine the orientation/"spin" 
          of the camera given its origin/target.
   * vfov: angle of the vertical field of view in degrees
   * aspect: aspect of the resulting image
   */
  camera(vec3 lookfrom, vec3 lookat, vec3 vup, float vfov, float aspect) {
    vec3 u, v, w;
    float theta = vfov*M_PI/180;
    float half_height = tan(theta/2); // assuming z=-1 plane below
    float half_width = aspect * half_height;

    origin = lookfrom;
    w = unit_vector(lookfrom - lookat);
    u = unit_vector(cross(vup, w));
    v = cross(w, u);
    lower_left_corner = origin - half_width*u - half_height*v - w;
    horizontal = 2 * half_width * u;
    vertical = 2 * half_height * v;
  }

  ray get_ray(float s, float t) {
    return ray(origin, lower_left_corner + s*horizontal + t*vertical - origin);
  }

  vec3 origin;
  vec3 lower_left_corner;
  vec3 horizontal;
  vec3 vertical;
};

#endif
