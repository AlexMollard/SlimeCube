#pragma once
#include "Entity.h"
class PointLight : public Entity
{
public:
	PointLight(std::string name, glm::vec3 pos);
	~PointLight();

	// Set Functions
	//----------------
	void SetAttributes(glm::vec3 newAmbient = glm::vec3(0.2f), glm::vec3 newAlbedo = glm::vec3(1.0, 0.75, 0.5), glm::vec3 newSpecular = glm::vec3(1.0f), float newConstant = 1.0f, float newLinear = 0.09f, float newQuadratic = 0.032f);
	void SetAmbient(glm::vec3 newAmbient) { ambient = newAmbient; }
	void SetAlbedo(glm::vec3 newAlbedo) { albedo = newAlbedo; }
	void SetSpecular(glm::vec3 newSpecular) { specular = newSpecular; }

	void SetConstant(float newConstant) { constant = newConstant; }
	void SetLinear(float newLinear) { linear = newLinear; }
	void SetQuadratic(float newQuadratic) { quadratic = newQuadratic; }
	void SetStrength(float newStrength) { strength = newStrength; }

	// Get Functions
	//----------------
	glm::vec3 GetAmbient() { return ambient; }
	glm::vec3 GetAlbedo() { return albedo; }
	glm::vec3 GetSpecular() { return specular; }
	float GetStrength() { return strength; };

	float GetConstant() { return constant; }
	float GetLinear() { return linear; }
	float GetQuadratic() { return quadratic; }
	//protected:
		//glm::vec3 lightPosition = glm::vec3(0, 1, 0);
	glm::vec3 albedo = glm::vec3(1.0, 0.75, 0.5);
	glm::vec3 ambient = glm::vec3(1);
	glm::vec3 specular = glm::vec3(1);

	float strength = 1.0f;
	glm::vec3 pos = glm::vec3(0.0f);
	float constant = 1.0f;
	float linear = 0.09f;
	float quadratic = 0.032f;
};
