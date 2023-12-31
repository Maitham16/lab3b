// Maitham Al-rubaye - A12142043 - Lab 3B - main function
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>
#include <limits>
#include <algorithm>
#include <utility>
#include "pugixml.hpp"
#include "stb_image.h"

#include "classes/Vector3.h"
#include "classes/Ray.h"
#include "classes/Material.h"
#include "classes/Sphere.h"
#include "classes/Light.h"
#include "classes/Camera.h"
#include "classes/Model.h"
#include "classes/RayTrace.h"
#include "classes/Scene.h"
#include "classes/render.h"
#include "classes/Transform.h"
#include "classes/Spotlight.h"

// Parse the XML file
// Sphere parsing
std::vector<Sphere> parseSpheres(const pugi::xml_node &surfacesNode)
{
    std::vector<Sphere> spheres;

    for (auto &&node : surfacesNode.children())
    {
        if (std::string(node.name()) == "sphere")
        {
            double radius = node.attribute("radius").as_double();
            pugi::xml_node positionNode = node.child("position");
            double x = positionNode.attribute("x").as_double();
            double y = positionNode.attribute("y").as_double();
            double z = positionNode.attribute("z").as_double();
            pugi::xml_node materialNode = node.child("material_solid");

            if (materialNode)
            {
                // Parse solid material
                pugi::xml_node colorNode = materialNode.child("color");
                double r = colorNode.attribute("r").as_double();
                double g = colorNode.attribute("g").as_double();
                double b = colorNode.attribute("b").as_double();
                pugi::xml_node phongNode = materialNode.child("phong");
                double ka = phongNode.attribute("ka").as_double();
                double kd = phongNode.attribute("kd").as_double();
                double ks = phongNode.attribute("ks").as_double();
                double exponent = phongNode.attribute("exponent").as_double();
                pugi::xml_node reflectanceNode = materialNode.child("reflectance");
                double reflectance = reflectanceNode.attribute("r").as_double();
                pugi::xml_node transmittanceNode = materialNode.child("transmittance");
                double transmittance = transmittanceNode.attribute("t").as_double();
                pugi::xml_node refractionNode = materialNode.child("refraction");
                double iof = refractionNode.attribute("iof").as_double();

                // save the attributes to the sphere
                Vector3 position(x, y, z);
                Sphere sphere(position, radius, Material());
                sphere.material.color = Vector3(r, g, b);
                sphere.material.ka = ka;
                sphere.material.kd = kd;
                sphere.material.ks = ks;
                sphere.material.exponent = exponent;
                sphere.material.reflectance = reflectance;
                sphere.material.transmittance = transmittance;
                sphere.material.refraction_index = iof;

                // Parse transformations
                pugi::xml_node transformNode = node.child("transform");
                if (transformNode)
                {
                    for (auto &&transform : transformNode.children())
                    {
                        std::string transformName = transform.name();

                        if (transformName == "translate")
                        {
                            double tx = transform.attribute("x").as_double();
                            double ty = transform.attribute("y").as_double();
                            double tz = transform.attribute("z").as_double();
                            // Apply translation
                            sphere.transform.translate(tx, ty, tz);
                        }
                        else if (transformName == "rotateX")
                        {
                            double angle = transform.attribute("theta").as_double();
                            // Apply rotation around X-axis
                            sphere.transform.rotateX(angle);
                        }
                        else if (transformName == "rotateY")
                        {
                            double angle = transform.attribute("theta").as_double();
                            // Apply rotation around Y-axis
                            sphere.transform.rotateY(angle);
                        }
                        else if (transformName == "rotateZ")
                        {
                            double angle = transform.attribute("theta").as_double();
                            // Apply rotation around Z-axis
                            sphere.transform.rotateZ(angle);
                        }
                        else if (transformName == "scale")
                        {
                            double sx = transform.attribute("x").as_double();
                            double sy = transform.attribute("y").as_double();
                            double sz = transform.attribute("z").as_double();
                            // Apply scaling
                            sphere.transform.scale(sx, sy, sz);
                        }
                    }
                }

                // Add the sphere to the vector
                spheres.push_back(sphere);

                // Console checking
                std::cout << "Sphere: Radius=" << radius << ", Position=(" << x << "," << y << "," << z << ")\n";
            }
            else
            {
                materialNode = node.child("material_textured");
                if (materialNode)
                {
                    // Parse textured material
                    pugi::xml_node textureNode = materialNode.child("texture");
                    std::string textureName = textureNode.attribute("name").as_string();
                    pugi::xml_node phongNode = materialNode.child("phong");
                    double ka = phongNode.attribute("ka").as_double();
                    double kd = phongNode.attribute("kd").as_double();
                    double ks = phongNode.attribute("ks").as_double();
                    double exponent = phongNode.attribute("exponent").as_double();
                    pugi::xml_node reflectanceNode = materialNode.child("reflectance");
                    double reflectance = reflectanceNode.attribute("r").as_double();
                    pugi::xml_node transmittanceNode = materialNode.child("transmittance");
                    double transmittance = transmittanceNode.attribute("t").as_double();
                    pugi::xml_node refractionNode = materialNode.child("refraction");
                    double iof = refractionNode.attribute("iof").as_double();

                    // save the attributes to the sphere
                    Vector3 position(x, y, z);
                    Sphere sphere(position, radius, Material(textureName, ka, kd, ks, exponent, reflectance, transmittance, iof));

                    // Parse transformations
                    pugi::xml_node transformNode = node.child("transform");
                    if (transformNode)
                    {
                        for (auto &&transform : transformNode.children())
                        {
                            std::string transformName = transform.name();

                            if (transformName == "translate")
                            {
                                double tx = transform.attribute("x").as_double();
                                double ty = transform.attribute("y").as_double();
                                double tz = transform.attribute("z").as_double();
                                // Apply translation
                                sphere.transform.translate(tx, ty, tz);
                            }
                            else if (transformName == "rotateX")
                            {
                                double angle = transform.attribute("theta").as_double();
                                // Apply rotation around X-axis
                                sphere.transform.rotateX(angle);
                            }
                            else if (transformName == "rotateY")
                            {
                                double angle = transform.attribute("theta").as_double();
                                // Apply rotation around Y-axis
                                sphere.transform.rotateY(angle);
                            }
                            else if (transformName == "rotateZ")
                            {
                                double angle = transform.attribute("theta").as_double();
                                // Apply rotation around Z-axis
                                sphere.transform.rotateZ(angle);
                            }
                            else if (transformName == "scale")
                            {
                                double sx = transform.attribute("x").as_double();
                                double sy = transform.attribute("y").as_double();
                                double sz = transform.attribute("z").as_double();
                                // Apply scaling
                                sphere.transform.scale(sx, sy, sz);
                            }
                        }
                    }

                    // Add the sphere to the vector
                    spheres.push_back(sphere);

                    // Console checking
                    std::cout << "Sphere: Radius=" << radius << ", Position=(" << x << "," << y << "," << z << ")\n";
                }
            }
        }
    }

    return spheres;
}

// Parse Material Solid
Material parseSolidMaterial(const pugi::xml_node &materialNode)
{
    pugi::xml_node colorNode = materialNode.child("color");
    double r = colorNode.attribute("r").as_double();
    double g = colorNode.attribute("g").as_double();
    double b = colorNode.attribute("b").as_double();
    pugi::xml_node phongNode = materialNode.child("phong");
    double ka = phongNode.attribute("ka").as_double();
    double kd = phongNode.attribute("kd").as_double();
    double ks = phongNode.attribute("ks").as_double();
    double exponent = phongNode.attribute("exponent").as_double();
    pugi::xml_node reflectanceNode = materialNode.child("reflectance");
    double reflectance = reflectanceNode.attribute("r").as_double();
    pugi::xml_node transmittanceNode = materialNode.child("transmittance");
    double transmittance = transmittanceNode.attribute("t").as_double();
    pugi::xml_node refractionNode = materialNode.child("refraction");
    double iof = refractionNode.attribute("iof").as_double();

    return Material(Vector3(r, g, b), ka, kd, ks, exponent, reflectance, transmittance, iof);
}

// Parse Material Textured
Material parseTexturedMaterial(const pugi::xml_node &materialNode)
{
    pugi::xml_node textureNode = materialNode.child("texture");
    std::string textureName = textureNode.attribute("name").as_string();
    pugi::xml_node phongNode = materialNode.child("phong");
    double ka = phongNode.attribute("ka").as_double();
    double kd = phongNode.attribute("kd").as_double();
    double ks = phongNode.attribute("ks").as_double();
    double exponent = phongNode.attribute("exponent").as_double();
    pugi::xml_node reflectanceNode = materialNode.child("reflectance");
    double reflectance = reflectanceNode.attribute("r").as_double();
    pugi::xml_node transmittanceNode = materialNode.child("transmittance");
    double transmittance = transmittanceNode.attribute("t").as_double();
    pugi::xml_node refractionNode = materialNode.child("refraction");
    double iof = refractionNode.attribute("iof").as_double();

    // console checking
    std::cout << "Texture: " << textureName << std::endl;
    std::cout << "Phong: ka=" << ka << ", kd=" << kd << ", ks=" << ks << ", exponent=" << exponent << std::endl;

    return Material(textureName, ka, kd, ks, exponent, reflectance, transmittance, iof);
}

// Parse Transformations
Transform parseTransform(const pugi::xml_node &transformNode)
{
    Transform transform;

    for (auto &&node : transformNode.children())
    {
        std::string nodeName = node.name();

        if (nodeName == "translate")
        {
            double x = node.attribute("x").as_double();
            double y = node.attribute("y").as_double();
            double z = node.attribute("z").as_double();
            transform.translate(x, y, z);
        }
        else if (nodeName == "rotateX")
        {
            double theta = node.attribute("theta").as_double();
            transform.rotateX(theta);
        }
        else if (nodeName == "rotateY")
        {
            double theta = node.attribute("theta").as_double();
            transform.rotateY(theta);
        }
        else if (nodeName == "rotateZ")
        {
            double theta = node.attribute("theta").as_double();
            transform.rotateZ(theta);
        }
        else if (nodeName == "scale")
        {
            double x = node.attribute("x").as_double();
            double y = node.attribute("y").as_double();
            double z = node.attribute("z").as_double();
            transform.scale(x, y, z);
        }
    }

    return transform;
}

// Parse Model (surface - mesh)
std::vector<Model> parseModels(const pugi::xml_node &surfacesNode)
{
    std::vector<Model> models;

    for (auto &&node : surfacesNode.children())
    {
        if (std::string(node.name()) == "mesh")
        {
            std::string meshName = node.attribute("name").as_string();
            pugi::xml_node materialNode = node.child("material_solid");

            if (materialNode)
            {
                // Parse solid material
                Material material = parseSolidMaterial(materialNode);
                Model model(meshName, material);

                // Parse transform
                pugi::xml_node transformNode = node.child("transform");
                if (transformNode)
                {
                    Transform transform = parseTransform(transformNode);
                    model.setTransform(transform);
                }

                models.push_back(model);
            }
            else
            {
                // Parse textured material
                materialNode = node.child("material_textured");
                Material material = parseTexturedMaterial(materialNode);
                Model model(meshName, material);
                models.push_back(model);

                // Parse transform
                pugi::xml_node transformNode = node.child("transform");
                if (transformNode)
                {
                    Transform transform = parseTransform(transformNode);
                    model.setTransform(transform);
                }
                // console checking
                std::cout << "Model: Name=" << meshName << std::endl;
                models.push_back(model);
            }
        }
    }
    return models;
}

// light parsing function
std::vector<Light> parseLights(const pugi::xml_node &lightsNode)
{
    std::vector<Light> lights;

    for (auto &&node : lightsNode.children())
    {
        if (std::string(node.name()) == "ambient_light")
        {
            pugi::xml_node colorNode = node.child("color");
            double r = colorNode.attribute("r").as_double();
            double g = colorNode.attribute("g").as_double();
            double b = colorNode.attribute("b").as_double();

            // save the attributes to the light
            Vector3 intensity(r, g, b);
            Light light(Vector3(), intensity, LightType::AMBIENT);

            // add the light to the vector
            lights.push_back(light);
            // console checking
            std::cout << "Ambient Light: Color=(" << r << "," << g << "," << b << ")\n";
        }
        else if (std::string(node.name()) == "point_light")
        {
            pugi::xml_node colorNode = node.child("color");
            double r = colorNode.attribute("r").as_double();
            double g = colorNode.attribute("g").as_double();
            double b = colorNode.attribute("b").as_double();
            pugi::xml_node positionNode = node.child("position");
            double x = positionNode.attribute("x").as_double();
            double y = positionNode.attribute("y").as_double();
            double z = positionNode.attribute("z").as_double();

            // save the attributes to the light
            Vector3 position(x, y, z);
            Vector3 intensity(r, g, b);
            Light light(position, intensity, LightType::POINT);

            // add the light to the vector
            lights.push_back(light);
            // console checking
            std::cout << "Point Light: Color=(" << r << "," << g << "," << b << "), Position=(" << x << "," << y << "," << z << ")\n";
        }
    }
    return lights;
}

// camera parsing function
Camera parseCamera(const pugi::xml_node &cameraNode)
{
    pugi::xml_node positionNode = cameraNode.child("position");
    double x = positionNode.attribute("x").as_double();
    double y = positionNode.attribute("y").as_double();
    double z = positionNode.attribute("z").as_double();
    pugi::xml_node lookatNode = cameraNode.child("lookat");
    double lookat_x = lookatNode.attribute("x").as_double();
    double lookat_y = lookatNode.attribute("y").as_double();
    double lookat_z = lookatNode.attribute("z").as_double();
    pugi::xml_node upNode = cameraNode.child("up");
    double up_x = upNode.attribute("x").as_double();
    double up_y = upNode.attribute("y").as_double();
    double up_z = upNode.attribute("z").as_double();
    pugi::xml_node horizontal_fovNode = cameraNode.child("horizontal_fov");
    double horizontal_fov = horizontal_fovNode.attribute("angle").as_double();
    pugi::xml_node resolutionNode = cameraNode.child("resolution");
    double horizontal = resolutionNode.attribute("horizontal").as_double();
    double vertical = resolutionNode.attribute("vertical").as_double();
    pugi::xml_node max_bouncesNode = cameraNode.child("max_bounces");
    double max_bounces = max_bouncesNode.attribute("n").as_double();

    // save the attributes to the camera
    Camera camera;
    camera.position = Vector3(x, y, z);
    camera.lookAt = Vector3(lookat_x, lookat_y, lookat_z);
    camera.up = Vector3(up_x, up_y, up_z);
    camera.fov = horizontal_fov;
    camera.imgWidth = static_cast<int>(horizontal);
    camera.imgHeight = static_cast<int>(vertical);
    camera.maxBounce = static_cast<int>(max_bounces);

    return camera;
}

// scene parsing function
Scene parseScene(const std::string &filename)
{
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(filename.c_str());

    if (!result)
    {
        std::cerr << "Error parsing XML: " << result.description() << std::endl;
        exit(EXIT_FAILURE);
    }

    pugi::xml_node sceneNode = doc.child("scene");
    if (!sceneNode)
    {
        std::cerr << "Error: No 'scene' node found in XML." << std::endl;
        exit(EXIT_FAILURE);
    }

    pugi::xml_node surfacesNode = sceneNode.child("surfaces");
    std::vector<Sphere> spheres = parseSpheres(surfacesNode);
    std::vector<Model> models = parseModels(surfacesNode);

    pugi::xml_node lightsNode = sceneNode.child("lights");
    std::vector<Light> lights = parseLights(lightsNode);

    pugi::xml_node cameraNode = sceneNode.child("camera");
    Camera camera = parseCamera(cameraNode);

    Scene scene;
    scene.spheres = spheres;
    scene.models = models;
    scene.lights = lights;
    scene.camera = camera;

    return scene;
}

//  main function
int main()
{
    // Prompt the user to enter the file number
    int fileNumber;
    std::cout << "Enter the file number (1-9): ";
    std::cin >> fileNumber;

    // Construct the file name
    std::stringstream fileNameStream;
    fileNameStream << "scenes/example" << fileNumber << ".xml";
    std::string fileName = fileNameStream.str();

    // Parse the scene from the XML file
    Scene scene = parseScene(fileName);

    scene.camera.transform.makeTranslation(-1.0, 1.0, 3.0);

    // Ask the user for isTransform
    std::cout << "Use camera transform? (y/n): ";
    std::string isTransformStr;
    std::cin >> isTransformStr;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    bool ans = (isTransformStr == "y") ? true : false;
    scene.camera.isTransform = ans;

    // Ask the user for dof
    std::cout << "Use depth of field? (y/n): ";
    std::string dofStr;
    std::cin >> dofStr;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    ans = (dofStr == "y") ? true : false;
    scene.camera.dof = ans;

    // turn on/off spotlight
    bool isSpotLight = false;

    if (isSpotLight)
    {
        // Create a spotlight
        Vector3 position(0.0, 3.0, -2.0);
        Vector3 direction = (scene.spheres[0].center - position).normalized();
        double angle = 10.0;
        Vector3 color(0.7, 0.7, 0.7);
        double intensity = 10.0;
        double falloffExponent = 2.0;

        Spotlight spotlight(position, direction, angle, color, intensity, falloffExponent);
        scene.addSpotlight(spotlight);
    }

    // Render the scene to an image
    render(scene, scene.camera);

    return 0;
}
