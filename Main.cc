#include "color.h"
#include "ray.h"
#include "vec3.h"
#include "sphere.h"

#include <iostream>
#include <fstream>

color ray_color(const ray& r) {
	//sphere hit
	hit_record hr;
	auto s = sphere(point3(0, 0, -1), 0.5);
	auto hit = s.hit(r, 0.0, 10.0, hr);
	if (hit) {
		vec3 N = unit_vector(hr.p - vec3(0,0,-1));
		return 0.5*color(N.x()+1, N.y()+1, N.z()+1);
	}

	//background hit
	vec3 unit_direction = unit_vector(r.direction());
	auto t = 0.5*(unit_direction.y() + 1.0);
	return (1.0 - t)*color(1.0, 1.0, 1.0) + t*color(0.5, 0.7, 1.0);
}

int main() 
{
	// Image 
	const auto aspect_ratio = 16.0 / 9.0;
	const int image_width = 400;
	const int image_height = static_cast<int>(image_width / aspect_ratio);

	// Camera

	auto viewport_height = 2.0;
	auto viewport_width = aspect_ratio * viewport_height;
	auto focal_length = 1.0;

	auto origin = point3(0, 0, 0);
	auto horizontal = vec3(viewport_width, 0, 0);
	auto vertical = vec3(0, viewport_height, 0);
	auto lower_left_corner = origin - horizontal/2 - vertical/2 - vec3(0, 0, focal_length);

	// Render

	std::ofstream imageStream("image.ppm");
	imageStream << "P3\n" << image_width << ' ' << image_height << "\n255\n";

	for (int j = image_height; j >= 0; j--) 
	{
		auto progress = double(100 * (image_height - j)) / image_height;
		std::cerr << "\rProgress: " << progress << "%    " << std::flush;
		for (int i = 0; i < image_width; i++)
		{
			auto u = double(i) / (image_width-1);
			auto v = double(j) / (image_height-1);
			ray r(origin, lower_left_corner + u*horizontal + v*vertical - origin);
			color pixel_color = ray_color(r);
			write_color(imageStream, pixel_color);
		}
		
	}

	imageStream.flush();
	imageStream.close();
	std::cerr << "\nDone!\n";
}