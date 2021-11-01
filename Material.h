#pragma once
#include "glm.hpp"
#include "Texture.h"

class Material
{
public:

	// Constructors (Subject to change)
	//-----------------
	Material(std::string name, Texture* albedo, Texture* specMap, Texture* normalMap, Texture* ambientMap, Texture* roughMap, Texture* displacementMap);
	Material(std::string name, Texture* albedo, float diffuseStrength, Texture* specMap, float specularStrength, Texture* normalMap, float normalStrength, Texture* ambientMap, float ambientStrength, Texture* roughMap, float roughStrength, Texture* displacementMap, float displacementStrength);
	Material(std::string name, Texture* albedo);

	virtual ~Material();

	// Textures
	//----------------
	// Set
	void SetAlbedo(Texture* newAlbedo);
	void SetSpecMap(Texture* newSpecular);
	void SetNormalMap(Texture* newNormal);
	void SetAmbientMap(Texture* newAmbient);
	void SetRoughMap(Texture* newRough);
	void SetDisplacementMap(Texture* newDisplacement);

	// Get
	Texture* GetAlbedo();
	Texture* GetSpecMap();
	Texture* GetNormalMap();
	Texture* GetAmbientMap();
	Texture* GetRoughMap();
	Texture* GetDisplacementMap();

	// Texture Strengths
	//----------------
	// Set
	void SetAlbedoStrength(float value) { diffuseStrength = value; }
	void SetSpecularStrength(float value) { specularStrength = value; }
	void SetNormalStrength(float value) { normalStrength = value; }
	void SetAmbientStrength(float value) { ambientStrength = value; }
	void SetRoughStrength(float value) { roughStrength = value; }
	void SetDisplacementStrength(float value) { displacementStrength = value; }
	void SetAll(Texture* albedo, Texture* specular, Texture* normal, Texture* ambient, Texture* rough, Texture* displacement, float diffuseStr, float specularStr, float normalStr, float ambientStr, float roughStr, float displaceStr);

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

	// Misc Functions
	//------------------
	void SetName(std::string newName) { name = newName; }
	std::string GetName() { return name.c_str(); }

protected:
	// Misc
	std::string name = "DefaultName";

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
	Texture* albedo = nullptr;
	Texture* specMap = nullptr;
	Texture* normalMap = nullptr;
	Texture* ambientMap = nullptr;
	Texture* roughMap = nullptr;
	Texture* displacementMap = nullptr;
};