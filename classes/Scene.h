#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include "Sphere.h"
#include "Light.h"
#include "Model.h"
#include "Camera.h"

class Scene
{
public:
    std::vector<Sphere> spheres;
    std::vector<Light> lights;
    std::vector<Model> models;
    Camera camera;

    void add(const Sphere& sphere)
    {
        spheres.push_back(sphere);
    }

    void add(const Light& light)
    {
        lights.push_back(light);
    }

    void add(const Model& model)
    {
        models.push_back(model);
    }

    void setCamera(const Camera& cam)
    {
        camera = cam;
    }

    bool intersect(const Ray& ray, float& t, Vector3& point, Vector3& normal, Material& material) const
    {
        for (const auto& sphere : spheres)
        {
            float t_sphere;
            if (sphere.intersect(ray, t_sphere))
            {
                if (t_sphere < t)
                {
                    t = t_sphere;
                    point = ray.origin + ray.direction * t;
                    normal = (point - sphere.center).normalized();
                    material = sphere.material;
                }
            }
        }
        for (const auto& model : models)
        {
            for (const auto& triangle : model.triangles)
            {
                float t_triangle, u, v;
                if (triangle.intersect(ray, 0.001f, std::numeric_limits<float>::max(), t_triangle, u, v))
                {
                    if (t_triangle < t)
                    {
                        t = t_triangle;
                        point = ray.origin + ray.direction * t;
                        normal = triangle.calculateNormal(u, v);
                        material = triangle.material;
                    }
                }
            }
        }
        return t < std::numeric_limits<float>::max();
    }
};

#endif
