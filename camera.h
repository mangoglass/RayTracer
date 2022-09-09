#ifndef CAMERA_H
#define CAMERA_H

#include "utility.h"

class camera {
    public:
        camera(
            point3 look_from, 
            point3 look_at, 
            vec3 up, 
            double vfov, // vertical fov in degrees
            double aspect_ratio, 
            double aperture, 
            double focus
        ) {
            auto theta = degrees_to_radians(vfov);
            auto h = tan(theta / 2);
            auto viewport_height = 2.0 * h;
            auto viewport_width = aspect_ratio * viewport_height;

            w = unit_vector(look_from - look_at);
            u = unit_vector(cross(up, w));
            v = cross(w, u);

            origin = look_from;
            horizontal = focus * viewport_width * u;
            vertical = focus * viewport_height * v;
            lower_left_corner = origin - horizontal/2 - vertical/2 - focus*w;

            lens_radius = aperture / 2;
        }

        ray get_ray(double s, double t) const {
            vec3 rd = lens_radius * random_in_unit_disk();
            vec3 offset = u * rd.x() + v * rd.y();

            return ray(origin + offset, lower_left_corner + s*horizontal + t*vertical - origin - offset);
        }

    private:
        point3 origin;
        point3 lower_left_corner;
        vec3 horizontal;
        vec3 vertical;
        vec3 u, v, w;
        double lens_radius;
};

#endif