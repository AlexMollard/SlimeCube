#pragma once

#include "Skybox.h"

class Material : public Resource
{
public:
 // Constructors (Subject to change)
 //-----------------
 Material(const std::string &name, const std::shared_ptr<Texture> &albedo, const std::shared_ptr<Texture> &specMap,
     const std::shared_ptr<Texture> &normalMap, const std::shared_ptr<Texture> &ambientMap,
     const std::shared_ptr<Texture> &roughMap, const std::shared_ptr<Texture> &displacementMap);

 Material(const std::string &name, const std::shared_ptr<Texture> &albedo, float diffuseStrength,
     const std::shared_ptr<Texture> &specMap, float specularStrength, const std::shared_ptr<Texture> &normalMap,
     float normalStrength, const std::shared_ptr<Texture> &ambientMap, float ambientStrength,
     const std::shared_ptr<Texture> &roughMap, float roughStrength, const std::shared_ptr<Texture> &displacementMap,
     float displacementStrength);

 Material(const std::string &name, std::shared_ptr<Texture> albedo);

 Material(const std::string &dir, void *args);

 void *Create();

 ~Material() override;

 // Textures
 //----------------
 // Set
 void SetAlbedo(const std::shared_ptr<Texture> &newAlbedo);

 void SetSpecMap(const std::shared_ptr<Texture> &newSpecular);

 void SetNormalMap(const std::shared_ptr<Texture> &newNormal);

 void SetAmbientMap(const std::shared_ptr<Texture> &newAmbient);

 void SetRoughMap(const std::shared_ptr<Texture> &newRough);

 void SetDisplacementMap(const std::shared_ptr<Texture> &newDisplacement);

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

 void SetAll(const std::shared_ptr<Texture> &albedo = nullptr, const std::shared_ptr<Texture> &specular = nullptr,
     const std::shared_ptr<Texture> &normal = nullptr, const std::shared_ptr<Texture> &ambient = nullptr,
     const std::shared_ptr<Texture> &rough = nullptr, const std::shared_ptr<Texture> &displacement = nullptr,
     float diffuseStr = 1.0f, float specularStr = 1.0f, float normalStr = 1.0f, float ambientStr = 1.0f, float roughStr = 1.0f,
     float displaceStr = 1.0f);

 // Get
 float GetAlbedoStrength() const { return diffuseStrength; }

 float GetSpecularStrength() const { return specularStrength; }

 float GetNormalStrength() const { return normalStrength; }

 float GetAmbientStrength() const { return ambientStrength; }

 float GetRoughStrength() const { return roughStrength; }

 float GetDisplacementStrength() const { return displacementStrength; }

 // Material Attributes (Outdated mainly used in phong lighting)
 // -------------------
 void setShininess(float newShininess) { shininess = newShininess; }

 float GetShininess() const { return shininess; }

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