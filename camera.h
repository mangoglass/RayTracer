#ifndef CAMERA_H
#define CAMERA_H

#include "utility.h"

class camera {
    public:
        camera(point3 look_from, point3 look_at, vec3 up, double vfov, double aspect_ratio) { // vfov is vertical fov in degrees
            auto theta = degrees_to_radians(vfov);
            auto h = tan(theta / 2);
            auto viewport_height = 2.0 * h;
            auto viewport_width = aspect_ratio * viewport_height;

            auto w = unit_vector(look_from - look_at);
            auto u = unit_vector(cross(up, w));
            auto v = cross(w, u);

            origin = look_from;
            horizontal = viewport_width * u;
            vertical = viewport_height * v;
            lower_left_corner = origin - horizontal/2 - vertical/2 - w;
        }

        ray get_ray(double s, double t) const {
            return ray(origin, lower_left_corner + s*horizontal + t*vertical - origin);
        }

    private:
        point3 origin;
        point3 lower_left_corner;
        vec3 horizontal;
        vec3 vertical;
};

#endif