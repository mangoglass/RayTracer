#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h"
#include "utility.h"

struct hit_record;

class material {
    public:
        virtual bool scatter(const ray& r_in, const hit_record& rec, color& attuenuation, ray& scattered) const = 0;
};

class lambertian : public material {
    public:
        lambertian(const color& a) : albedo(a) {}

        virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
            vec3 scatter_direction = rec.normal + random_unit_vector();

            // Catch very short directions that can lead to errors
            if (scatter_direction.near_zero()) scatter_direction = rec.normal;

            scattered = ray(rec.p, scatter_direction);
            attenuation = albedo;
            return true;
        }

    public:
        color albedo;
};

class metal : public material {
    public:
        metal(const color& a, double f) : albedo(a), fuzz(f) {}

        virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
            vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
            scattered = ray(rec.p, reflected + fuzz*random_in_unit_sphere());
            attenuation = albedo;

            return (dot(scattered.direction(), rec.normal) > 0);
        }

    public:
        color albedo;
        double fuzz;
};

class dielectric : public material {
    public:
        dielectric(double index_of_refreaction) : ir(index_of_refreaction) {}

        virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
            attenuation = color(1.0, 1.0, 1.0);
            double refraction_ratio = rec.front_face ? (1.0 / ir) : ir;

            vec3 unit_direction = unit_vector(r_in.direction());
            double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
            double sin_theta = sqrt(1.0 - cos_theta*cos_theta);

            bool cannot_refract = refraction_ratio * sin_theta > 1.0;
            bool block_refract = reflectance(cos_theta, refraction_ratio) > random_double();
            vec3 direction;

            if (cannot_refract || block_refract) direction = reflect(unit_direction, rec.normal);
            else direction = refract(unit_direction, rec.normal, refraction_ratio);

            scattered = ray(rec.p, direction);
            return true;
        }

    public:
        double ir; // index of refraction

    private:
        static double reflectance(double cosine, double ref_idx) {
            // Use Schlick's approximation for reflectance.
            auto r0 = (1 - ref_idx) / (1 + ref_idx);
            r0 = r0*r0;
            return r0 + (1 - r0) * pow((1 - cosine), 5);
        }
};

#endif