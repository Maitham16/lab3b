// header class for Ray Tracing
#ifndef RAYTRACE_H
#define RAYTRACE_H

#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include <random>
#include <algorithm>
#include "Vector3.h"
#include "Ray.h"
#include "Sphere.h"
#include "Scene.h"
#include "Material.h"
#include "Light.h"

float clamp(float val, float min, float max)
{
    return std::max(min, std::min(max, val));
}

Vector3 ray_trace(const Ray &ray, const Scene &scene, const Camera &camera, int depth = 0)
{
    if (depth > camera.maxBounce)
        return Vector3(0.0, 0.0, 0.0); // Maximum recursion depth reached

    float t = std::numeric_limits<float>::max();
    Vector3 point, normal;
    Material hit_material;

    if (scene.intersect(ray, t, point, normal, hit_material))
    {
        Vector3 color(0.0, 0.0, 0.0);

        // Ambient light contribution
        Vector3 ambient_color = hit_material.color * hit_material.ka;
        color = color + ambient_color;

        for (const Light &light : scene.lights)
        {
            Vector3 light_dir = (light.position - point).normalized();

            // Shadow ray direction
            Vector3 shadow_dir = light_dir;

            // Shadow ray origin with bias
            Vector3 shadow_origin = point + shadow_dir * 0.001f; // Adjust the bias value as needed

            // Shadow ray
            Ray shadow_ray(shadow_origin, shadow_dir);

            // Check for shadows
            float shadow_t = std::numeric_limits<float>::max();
            Vector3 shadow_point, shadow_normal;
            Material shadow_material;

            if (scene.intersect(shadow_ray, shadow_t, shadow_point, shadow_normal, shadow_material))
            {
                // The point is in shadow, so no contribution from this light
                continue;
            }

            // Diffuse shading
            float diffuse_factor = std::max(0.0f, normal.dot(light_dir));
            Vector3 diffuse_color = hit_material.color * hit_material.kd * diffuse_factor;

            // Specular shading
            Vector3 reflect_dir = reflect(-light_dir, normal);
            Vector3 view_dir = (camera.position - point).normalized();
            float spec_factor = std::pow(std::max(0.0f, reflect_dir.dot(view_dir)), hit_material.exponent);
            Vector3 specular_color = hit_material.ks * light.intensity * spec_factor;

            // Attenuation
            float distance = (light.position - point).length();
            float attenuation = 1.0 / (1.0 + hit_material.ka * distance);

            // Accumulate color (diffuse and specular parts with attenuation)
            color = color + (diffuse_color + specular_color) * light.intensity * attenuation;
        }

        // Reflection calculation
        Vector3 reflect_dir = reflect(ray.direction, normal).normalized();
        Vector3 reflect_origin = point + reflect_dir * 0.001f; // Adjust the bias value as needed
        Ray reflected_ray(reflect_origin, reflect_dir);

        // Recursive ray tracing for reflected ray
        Vector3 reflected_color = ray_trace(reflected_ray, scene, camera, depth + 1);

        // Blend the color depending on the reflectance of the material
        color = (color * (1.0f - hit_material.reflectance)) + (reflected_color * hit_material.reflectance);

        Vector3 refraction_direction = refract(ray.direction, normal, hit_material.refraction_index).normalized();
        Vector3 refraction_origin = point + refraction_direction * 0.001f; // Adjust the bias value as needed
        Ray refracted_ray(refraction_origin, refraction_direction);

        // Recursive ray tracing for refracted ray
        Vector3 refracted_color = ray_trace(refracted_ray, scene, camera, depth + 1);

        // Blend the color depending on the transmittance of the material
        color = color * (1.0f - hit_material.transmittance) + refracted_color * hit_material.transmittance;

        // Calculate the final color using reflection and refraction
        color = (color * (1.0f - hit_material.reflectance - hit_material.transmittance)) + (reflected_color * hit_material.reflectance) + (refracted_color * hit_material.transmittance);
        // Texture mapping
        if (hit_material.texture && hit_material.texture->isValid()) // Check if the texture is valid
        {
            // Get the texture coordinates at the intersection point
            Vector2 texCoords = hit_material.textureCoordinates(point);

            // Sample the texture color at the texture coordinates
            Vector3 texture_color = hit_material.texture->sample(texCoords);

            // Apply the texture color to the material color
            color = color * texture_color;
        }

        return color;
    }
    else
    {
        return Vector3(0.0, 0.0, 0.0);
    }
}

#endif
