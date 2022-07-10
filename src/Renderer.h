#pragma once

#include "Components.h"
#include "Entity.h"

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

 static void DrawEntity(Entity entity);

 static void AddPointLight(const std::shared_ptr<Entity> light);

 static void RemovePointLight(const std::shared_ptr<Entity> light);

 Renderer() = default;

 ~Renderer() = default;

private:
 Renderer(const Renderer &) = delete;

 Renderer(Renderer &&) = delete;

 Renderer &operator=(const Renderer &) = delete;

 Renderer &operator=(Renderer &&) = delete;

 void BindTexture(std::shared_ptr<Shader> shader, TEXTURETYPE texType, std::shared_ptr<Texture> texture);

 void SetIntTexture(std::shared_ptr<Shader> shader, TEXTURETYPE texType);

 static void UpdateLights(std::shared_ptr<Shader> shader);

 std::shared_ptr<Texture> currentTexture[6] = {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};

 static void initSingleton();

 static std::shared_ptr<Renderer> instance;
 static std::once_flag initInstanceFlag;

 std::shared_ptr<Shader> currentShader;
 std::vector<std::shared_ptr<Entity>> pointLights;
};