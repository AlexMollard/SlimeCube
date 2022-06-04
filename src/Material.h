#pragma once

#include "glm/glm.hpp"
#include "Skybox.h"
#include <memory>
#include <string>

class Material : public Resource {
public:

    // Constructors (Subject to change)
    //-----------------
    Material(const std::string &name,
             std::shared_ptr<Texture> albedo,
             std::shared_ptr<Texture> specMap,
             std::shared_ptr<Texture> normalMap,
             std::shared_ptr<Texture> ambientMap,
             std::shared_ptr<Texture> roughMap,
             std::shared_ptr<Texture> displacementMap);

    Material(const std::string &name,
             std::shared_ptr<Texture> albedo, float diffuseStrength,
             std::shared_ptr<Texture> specMap, float specularStrength,
             std::shared_ptr<Texture> normalMap, float normalStrength,
             std::shared_ptr<Texture> ambientMap, float ambientStrength,
             std::shared_ptr<Texture> roughMap, float roughStrength,
             std::shared_ptr<Texture> displacementMap, float displacementStrength);

    Material(const std::string &name, std::shared_ptr<Texture> albedo);

    Material(std::string dir, void *args);

    void *Create();

    virtual ~Material();

    // Textures
    //----------------
    // Set
    void SetAlbedo(std::shared_ptr<Texture> newAlbedo);

    void SetSpecMap(std::shared_ptr<Texture> newSpecular);

    void SetNormalMap(std::shared_ptr<Texture> newNormal);

    void SetAmbientMap(std::shared_ptr<Texture> newAmbient);

    void SetRoughMap(std::shared_ptr<Texture> newRough);

    void SetDisplacementMap(std::shared_ptr<Texture> newDisplacement);

    // Get
    std::shared_ptr<Texture> GetAlbedo();

    std::shared_ptr<Texture> GetSpecMap();

    std::shared_ptr<Texture> GetNormalMap();

    std::shared_ptr<Texture> GetAmbientMap();

    std::shared_ptr<Texture> GetRoughMap();

    std::shared_ptr<Texture> GetDisplacementMap();

    // Texture Strengths
    //----------------
    // Set
    void SetAlbedoStrength(float value) { diffuseStrength = value; }

    void SetSpecularStrength(float value) { specularStrength = value; }

    void SetNormalStrength(float value) { normalStrength = value; }

    void SetAmbientStrength(float value) { ambientStrength = value; }

    void SetRoughStrength(float value) { roughStrength = value; }

    void SetDisplacementStrength(float value) { displacementStrength = value; }

    void SetAll(std::shared_ptr<Texture> albedo = nullptr,
                std::shared_ptr<Texture> specular = nullptr,
                std::shared_ptr<Texture> normal = nullptr,
                std::shared_ptr<Texture> ambient = nullptr,
                std::shared_ptr<Texture> rough = nullptr,
                std::shared_ptr<Texture> displacement = nullptr,
                float diffuseStr = 1.0f, float specularStr = 1.0f,
                float normalStr = 1.0f, float ambientStr = 1.0f,
                float roughStr = 1.0f, float displaceStr = 1.0f);

    // Get
    float GetAlbedoStrength() { return diffuseStrength; }

    float GetSpecularStrength() { return specularStrength; }

    float GetNormalStrength() { return normalStrength; }

    float GetAmbientStrength() { return ambientStrength; }

    float GetRoughStrength() { return roughStrength; }

    float GetDisplacementStrength() { return displacementStrength; }

    // Material Attributes (Outdated mainly used in phong lighting)
    // -------------------
    void setShininess(float newShininess) { shininess = newShininess; }

    float GetShininess() { return shininess; }

protected:
    // Phong Lighting variables
    float shininess = 1.0f;

    // Has Bools
    bool hasSpecMap = false;
    bool hasNormalMap = false;
    bool hasSpecColorMap = false;
    bool hasRoughMap = false;
    bool hasDisplacementMap = false;

    // Texture Strengths
    float diffuseStrength = 1.0f;
    float specularStrength = 1.0f;
    float normalStrength = 1.0f;
    float ambientStrength = 1.0f;
    float roughStrength = 1.0f;
    float displacementStrength = 1.0f;

    // TextureMaps
    std::shared_ptr<Texture> albedo = nullptr;
    std::shared_ptr<Texture> specMap = nullptr;
    std::shared_ptr<Texture> normalMap = nullptr;
    std::shared_ptr<Texture> ambientMap = nullptr;
    std::shared_ptr<Texture> roughMap = nullptr;
    std::shared_ptr<Texture> displacementMap = nullptr;
};