#include "Material.h"

Material::Material(const std::string &name, const std::shared_ptr<Texture> &albedo, const std::shared_ptr<Texture> &specMap,
    const std::shared_ptr<Texture> &normalMap, const std::shared_ptr<Texture> &ambientMap,
    const std::shared_ptr<Texture> &roughMap, const std::shared_ptr<Texture> &displacementMap)
{
 this->name = name;

 this->albedo = albedo;

 if (specMap) hasSpecMap = true;
 this->specMap = specMap;

 if (normalMap) hasNormalMap = true;
 this->normalMap = normalMap;

 if (ambientMap) hasSpecColorMap = true;
 this->ambientMap = ambientMap;

 if (ambientMap) hasRoughMap = true;
 this->roughMap = roughMap;

 if (displacementMap) hasRoughMap = true;
 this->displacementMap = displacementMap;
}

Material::Material(const std::string &name, const std::shared_ptr<Texture> &albedo, float diffuseStrength,
    const std::shared_ptr<Texture> &specMap, float specularStrength, const std::shared_ptr<Texture> &normalMap,
    float normalStrength, const std::shared_ptr<Texture> &ambientMap, float ambientStrength,
    const std::shared_ptr<Texture> &roughMap, float roughStrength, const std::shared_ptr<Texture> &displacementMap,
    float displacementStrength)
{
 this->name = name;

 this->albedo = albedo;

 if (specMap) hasSpecMap = true;
 this->specMap = specMap;

 if (normalMap) hasNormalMap = true;
 this->normalMap = normalMap;

 if (ambientMap) hasSpecColorMap = true;
 this->ambientMap = ambientMap;

 if (ambientMap) hasRoughMap = true;
 this->roughMap = roughMap;

 if (displacementMap) hasRoughMap = true;
 this->displacementMap = displacementMap;

 this->diffuseStrength = diffuseStrength;
 this->specularStrength = specularStrength;
 this->normalStrength = normalStrength;
 this->ambientStrength = ambientStrength;
 this->roughStrength = roughStrength;
 this->displacementStrength = displacementStrength;
}

Material::Material(const std::string &name, std::shared_ptr<Texture> albedo)
{
 this->name = name;

 this->albedo = std::move(albedo);
 this->specMap = nullptr;
 this->normalMap = nullptr;
 this->ambientMap = nullptr;
 this->roughMap = nullptr;
}

Material::Material(const std::string &dir, void *args) { Create(); }

void *Material::Create()
{
 // Open material file
 // Read File
 // Get all dir of textures
 // Load/check all textures
 // Set all textures
 SetAll();
 return this;
}

Material::~Material() = default;

std::shared_ptr<Texture> Material::GetAlbedo()
{
 if (albedo != nullptr) return albedo;

 return nullptr;
}

std::shared_ptr<Texture> Material::GetSpecMap()
{
 if (specMap != nullptr) return specMap;

 return nullptr;
}

std::shared_ptr<Texture> Material::GetNormalMap()
{
 if (normalMap != nullptr) return normalMap;

 return nullptr;
}

std::shared_ptr<Texture> Material::GetAmbientMap()
{
 if (ambientMap != nullptr) return ambientMap;

 return nullptr;
}

std::shared_ptr<Texture> Material::GetRoughMap()
{
 if (roughMap != nullptr) return roughMap;

 return nullptr;
}

std::shared_ptr<Texture> Material::GetDisplacementMap()
{
 if (displacementMap != nullptr) return displacementMap;

 return nullptr;
}

void Material::SetAll(const std::shared_ptr<Texture> &albedo, const std::shared_ptr<Texture> &specular,
    const std::shared_ptr<Texture> &normal, const std::shared_ptr<Texture> &ambient, const std::shared_ptr<Texture> &rough,
    const std::shared_ptr<Texture> &displacement, float diffuseStr, float specularStr, float normalStr, float ambientStr,
    float roughStr, float displacementStr)
{
 SetAlbedo(albedo);
 SetAlbedoStrength(diffuseStr);

 SetSpecMap(specular);
 SetSpecularStrength(specularStr);

 SetNormalMap(normal);
 SetNormalStrength(normalStr);

 SetAmbientMap(ambient);
 SetAmbientStrength(ambientStr);

 SetRoughMap(rough);
 SetRoughStrength(roughStr);

 SetDisplacementMap(displacement);
 SetDisplacementStrength(displacementStr);
}

void Material::SetAlbedo(const std::shared_ptr<Texture> &newAlbedo) { albedo = newAlbedo; }

void Material::SetSpecMap(const std::shared_ptr<Texture> &newSpecular) { specMap = newSpecular; }

void Material::SetNormalMap(const std::shared_ptr<Texture> &newNormal) { normalMap = newNormal; }

void Material::SetAmbientMap(const std::shared_ptr<Texture> &newAmbient) { ambientMap = newAmbient; }

void Material::SetRoughMap(const std::shared_ptr<Texture> &newRough) { roughMap = newRough; }

void Material::SetDisplacementMap(const std::shared_ptr<Texture> &newDisplacement) { displacementMap = newDisplacement; }