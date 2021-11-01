#include "Material.h"

Material::Material(std::string name, Texture* albedo, Texture* specMap, Texture* normalMap, Texture* ambientMap, Texture* roughMap, Texture* displacementMap)
{
	this->name = name;

	this->albedo = albedo;

	if (specMap)
		hasSpecMap = true;
	this->specMap = specMap;

	if (normalMap)
		hasNormalMap = true;
	this->normalMap = normalMap;

	if (ambientMap)
		hasSpecColorMap = true;
	this->ambientMap = ambientMap;

	if (ambientMap)
		hasRoughMap = true;
	this->roughMap = roughMap;

	if (displacementMap)
		hasRoughMap = true;
	this->displacementMap = displacementMap;
}

Material::Material(std::string name, Texture* albedo, float diffuseStrength, Texture* specMap, float specularStrength, Texture* normalMap, float normalStrength, Texture* ambientMap, float ambientStrength, Texture* roughMap, float roughStrength, Texture* displacementMap, float displacementStrength)
{
	this->name = name;

	this->albedo = albedo;

	if (specMap)
		hasSpecMap = true;
	this->specMap = specMap;

	if (normalMap)
		hasNormalMap = true;
	this->normalMap = normalMap;

	if (ambientMap)
		hasSpecColorMap = true;
	this->ambientMap = ambientMap;

	if (ambientMap)
		hasRoughMap = true;
	this->roughMap = roughMap;

	if (displacementMap)
		hasRoughMap = true;
	this->displacementMap = displacementMap;

	this->diffuseStrength = diffuseStrength;
	this->specularStrength = specularStrength;
	this->normalStrength = normalStrength;
	this->ambientStrength = ambientStrength;
	this->roughStrength = roughStrength;
	this->displacementStrength = displacementStrength;
}

Material::Material(std::string name, Texture* albedo)
{
	this->name = name;

	this->albedo = albedo;
	this->specMap = nullptr;
	this->normalMap = nullptr;
	this->ambientMap = nullptr;
	this->roughMap = nullptr;
}

Material::~Material()
{
}

Texture* Material::GetAlbedo()
{
	if (albedo != nullptr)
		return albedo;

	return nullptr;
}

Texture* Material::GetSpecMap()
{
	if (specMap != nullptr)
		return specMap;

	return nullptr;
}

Texture* Material::GetNormalMap()
{
	if (normalMap != nullptr)
		return normalMap;

	return nullptr;
}

Texture* Material::GetAmbientMap()
{
	if (ambientMap != nullptr)
		return ambientMap;

	return nullptr;
}

Texture* Material::GetRoughMap()
{
	if (roughMap != nullptr)
		return roughMap;

	return nullptr;
}

Texture* Material::GetDisplacementMap()
{
	if (displacementMap != nullptr)
		return displacementMap;

	return nullptr;
}

void Material::SetAll(Texture* albedo, Texture* specular, Texture* normal, Texture* ambient, Texture* rough, Texture* displacement, float diffuseStr, float specularStr, float normalStr, float ambientStr, float roughStr, float displacementStr)
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

void Material::SetAlbedo(Texture* newAlbedo)
{
	albedo = newAlbedo;
}

void Material::SetSpecMap(Texture* newSpecular)
{
	specMap = newSpecular;
}

void Material::SetNormalMap(Texture* newNormal)
{
	normalMap = newNormal;
}

void Material::SetAmbientMap(Texture* newAmbient)
{
	ambientMap = newAmbient;
}

void Material::SetRoughMap(Texture* newRough)
{
	roughMap = newRough;
}

void Material::SetDisplacementMap(Texture* newDisplacement)
{
	displacementMap = newDisplacement;
}