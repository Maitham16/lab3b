#ifndef RENDER_H
#define RENDER_H

#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include <random>
#include <algorithm>
#include <fstream>

#include "Vector3.h"
#include "Ray.h"
#include "Sphere.h"
#include "Scene.h"
#include "RayTrace.h"

void render(const Scene& scene, const Camera& camera) {
    const int width = camera.imgWidth;
    const int height = camera.imgHeight;
    Vector3 image[width][height];

    for (int j = 0; j < height; ++j) {
        for (int i = 0; i < width; ++i) {
            float u = float(i) / float(width);
            float v = float(j) / float(height);
            Ray ray = camera.generateRay(u, v);
            image[i][j] = ray_trace(ray, scene, camera);
        }
    }
// write to file
    std::ofstream ofs;
    ofs.open("./out.ppm");
    ofs << "P3\n" << width << " " << height << "\n255\n";
    for (int j = height - 1; j >= 0; --j) {
        for (int i = 0; i < width; ++i) {
            int ir = int(255.99 * image[i][j].x);
            int ig = int(255.99 * image[i][j].y);
            int ib = int(255.99 * image[i][j].z);
            ofs << ir << " " << ig << " " << ib << "\n";
        }
    }
    ofs.close();
}


#endif
