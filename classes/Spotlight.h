// spotlight header class
#ifndef SPOTLIGHT_H
#define SPOTLIGHT_H

#include "Vector3.h"
#include <cmath>

float clamp(float val, float min, float max)
{
    return std::max(min, std::min(max, val));
}

class Spotlight
{
public:
    Vector3 position;
    Vector3 direction;
    float angle;
    Vector3 color;
    float intensity;
    float falloffExponent;

    Spotlight() {}

    Spotlight(const Vector3& position, const Vector3& direction, float angle, const Vector3& color, float intensity, float falloffExponent)
    {
        this->position = position;
        this->direction = direction;
        this->angle = angle;
        this->color = color;
        this->intensity = intensity;
        this->falloffExponent = falloffExponent;
    }

    Vector3 getLighting(const Vector3& point) const
    {
        Vector3 lightDirection = (point - position).normalize();

        // Compute the cosine of the angle between the light direction and the spotlight direction
        float cosAngle = lightDirection.dot(direction);

        // Check if the point is within the cone of light
        if (cosAngle > cos(angle / 2.0))
        {
            // Compute the distance between the light source and the point
            float distance = (point - position).length();

            // Compute the attenuation factor
            float attenuation = 1.0 / (distance * distance);

            // Compute falloff factor
            float falloff = pow(clamp((1.0 - cosAngle/ angle), 0.0, 1.0), falloffExponent);

            // Return the light color scaled by the intensity, the attenuation and the falloff
            return color * intensity * attenuation * falloff;
        }
        else
        {
            // The point is outside the cone of light, so it receives no light from this source
            return Vector3(0, 0, 0);
        }
    }
};

#endif