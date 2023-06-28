#ifndef CAMERA_H
#define CAMERA_H

#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"
#include "Ray.h"
#include <cmath>

class Camera
{
public:
    Vector3 position;
    Vector3 lookAt;
    Vector3 up;
    double fov;
    int imgWidth;
    int imgHeight;
    int maxBounce;
    Matrix4 transform;
    bool isTransform;

    Camera()
    {
        position = Vector3(0, 0, 0);
        lookAt = Vector3(0, 0, 1);
        up = Vector3(0, 1, 0);
        fov = 60;
        imgWidth = 640;
        imgHeight = 480;
        maxBounce = 5;
        Matrix4 transform;
        isTransform = false; // Set to false by default
    }

    Camera(Vector3 position, Vector3 lookAt, Vector3 up, double fov, int imgWidth, int imgHeight, int maxBounce, bool isTransform)
    {
        this->position = position;
        this->lookAt = lookAt;
        this->up = up;
        this->fov = fov;
        this->imgWidth = imgWidth;
        this->imgHeight = imgHeight;
        this->maxBounce = maxBounce;
        this->isTransform = isTransform;
    }

    Ray generateRay(double u, double v) const
    {
        if (isTransform) // Check if the camera transformation is enabled
        {
            // Compute transformation matrix
            Vector3 forward = (this->lookAt - this->position).normalize();
            Vector3 right = forward.cross(this->up).normalize();
            Vector3 upNew = right.cross(forward).normalize();

            Matrix4 translationMatrix;
            translationMatrix.makeTranslation(-position.x, -position.y, -position.z);

            Matrix4 rotationMatrix;
            rotationMatrix.makeRotationFromBasis(right, upNew, -forward);

            // Generate ray in camera space
            double theta = fov;
            double halfWidth = std::tan(theta / 1.0);
            double halfHeight = halfWidth * (imgHeight / static_cast<double>(imgWidth));
            Vector3 rayOriginCameraSpace = Vector3(0, 0, 0);
            Vector3 rayDirectionCameraSpace = Vector3(2.0 * halfWidth * (u - 0.5), 2.0 * halfHeight * (v - 0.5), -1.0).normalize();

            // Transform to world space
            Vector4 rayOriginWorldSpace = transform * Vector4(rayOriginCameraSpace, 1.0);
            Vector4 rayDirectionWorldSpace = transform * Vector4(rayDirectionCameraSpace, 0.0);

            // Create and return the ray
            return Ray(Vector3(rayOriginWorldSpace.x, rayOriginWorldSpace.y, rayOriginWorldSpace.z), Vector3(rayDirectionWorldSpace.x, rayDirectionWorldSpace.y, rayDirectionWorldSpace.z).normalize());
        }
        else
        {

            // Calculate the direction vector based on the camera's position, lookAt, and up vectors
            Vector3 direction = (lookAt - position).normalize();

            // Calculate the right vector
            Vector3 right = direction.cross(up).normalize();

            // Calculate the up vector
            Vector3 camUp = right.cross(direction).normalize();

            // Calculate the field of view half-angles
            double theta = fov;
            double halfWidth = std::tan(theta / 1.0);
            double halfHeight = halfWidth * (imgHeight / static_cast<double>(imgWidth));

            // Calculate the ray direction
            Vector3 rayDirection = (direction + (right * (2.0 * halfWidth * (u - 0.4))) + (camUp * (2.0 * halfHeight * (v - 0.4)))).normalize();

            // Create and return the ray
            return Ray(position, rayDirection);
        }
    }
};

#endif
