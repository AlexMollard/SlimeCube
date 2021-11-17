#pragma once
#include "Entity.h"
#include "PointLight.h"

enum class TEXTURETYPE
{
	Albedo,
	Specular,
	Normal,
	Ambient,
	Rough,
	Displacement,
	SkyBox
};

class Renderer
{
public:
	static std::shared_ptr<Renderer> GetInstance();

	Renderer(const Renderer&) = delete;
	Renderer(Renderer&&) = delete;
	Renderer& operator=(const Renderer&) = delete;
	Renderer& operator=(Renderer&&) = delete;

	static void DrawEntity(Entity entity);
	Renderer()
	{
		pointLights.push_back(std::make_shared<PointLight>("temp",glm::vec3(0.0f)));
	}
	~Renderer() = default;
private:
	void BindTexture(std::shared_ptr<Shader> shader, TEXTURETYPE texType, std::shared_ptr<Texture> texture);
	void SetIntTexture(std::shared_ptr<Shader> shader,  TEXTURETYPE texType);
	static void UpdateLights(std::shared_ptr<Shader> shader);

	std::vector<std::shared_ptr<PointLight>> pointLights;
	std::shared_ptr<Texture> currentTexture[6] = { nullptr,nullptr,nullptr,nullptr,nullptr,nullptr };
	static std::shared_ptr<Renderer> instance;
	
};