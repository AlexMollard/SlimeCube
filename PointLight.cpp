#include "pch.h"
#include "PointLight.h"

PointLight::PointLight(std::string name, glm::vec3 pos)
{
	this->pos = pos;
	SetAttributes();
}

PointLight::~PointLight()
{
}

void PointLight::SetAttributes(glm::vec3 newAmbient, glm::vec3 newAlbedo, glm::vec3 newSpecular, float newConstant, float newLinear, float newQuadratic)
{
	SetAmbient(newAmbient);
	SetAlbedo(newAlbedo);
	SetSpecular(newSpecular);

	SetConstant(newConstant);
	SetLinear(newLinear);
	SetQuadratic(newQuadratic);
}