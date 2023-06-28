#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <iostream>
#include "Vector2.h"
#include "Vector3.h"
#include "Color.h"
#include <stb_image.h>

class Texture
{
public:
    std::string filename;
    Vector2 dimensions;
    Vector3 *data;

    Texture(const std::string &filename)
        : filename(filename), dimensions(Vector2(0.0, 0.0)), data(nullptr)
    {
        load();
    }

    void load()
    {
        // Load the texture data from the specified file using stb_image library
        int width, height, numChannels;
        unsigned char *imageData = stbi_load(filename.c_str(), &width, &height, &numChannels, 0);
        if (imageData == nullptr)
        {
            std::cerr << "Failed to load texture: " << filename << std::endl;
            return;
        }

        dimensions = Vector2(static_cast<float>(width), static_cast<float>(height));

        // Allocate memory for the texture data and populate it with the loaded data
        data = new Vector3[width * height];
        for (int y = 0; y < height; ++y)
        {
            for (int x = 0; x < width; ++x)
            {
                int index = y * width + x;
                int pixelIndex = (y * width + x) * numChannels;

                float r = imageData[pixelIndex] / 255.0f;
                float g = imageData[pixelIndex + 1] / 255.0f;
                float b = imageData[pixelIndex + 2] / 255.0f;

                data[index] = Vector3(r, g, b);
            }
        }

        stbi_image_free(imageData);
    }

    Vector3 sample(const Vector2 &texCoords) const
    {
        // Sample the texture at the specified texture coordinates using bilinear interpolation
        float x = texCoords.x * dimensions.x;
        float y = texCoords.y * dimensions.y;

        int x0 = static_cast<int>(x);
        int y0 = static_cast<int>(y);
        int x1 = std::min(x0 + 1, static_cast<int>(dimensions.x) - 1);
        int y1 = std::min(y0 + 1, static_cast<int>(dimensions.y) - 1);

        float dx = x - x0;
        float dy = y - y0;

        Vector3 c00 = getColor(x0, y0);
        Vector3 c10 = getColor(x1, y0);
        Vector3 c01 = getColor(x0, y1);
        Vector3 c11 = getColor(x1, y1);

        Vector3 c0 = lerp(c00, c10, dx);
        Vector3 c1 = lerp(c01, c11, dx);

        return lerp(c0, c1, dy);
    }

    bool isValid() const
    {
        // Return true if the texture is valid and loaded, false otherwise
        return (data != nullptr && dimensions.x > 0 && dimensions.y > 0);
    }

// getWidth and getHeight functions
    float getWidth() const
    {
        return dimensions.x;
    }

    float getHeight() const
    {
        return dimensions.y;
    }

private:
    Vector3 getColor(int x, int y) const
    {
        // Check if the indices are within the valid range
        if (x < 0 || x >= dimensions.x || y < 0 || y >= dimensions.y)
        {
            // Return a default color if the indices are out of bounds
            return Vector3(0.0f, 0.0f, 0.0f);
        }

        int index = y * static_cast<int>(dimensions.x) + x;
        return data[index];
    }

    Vector3 lerp(const Vector3 &a, const Vector3 &b, float t) const
    {
        return a * (1.0f - t) + b * t;
    }
};

#endif
