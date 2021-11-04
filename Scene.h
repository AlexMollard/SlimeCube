#pragma once
#include "Skybox.h"
#include "Camera.h"
#include "entt.hpp"

class SceneObject;
class Material;
class Shader;
class Mesh;

class Scene
{
public:
	Scene(Camera* cam);
	~Scene();

	void* Render(float deltaTime);

	SceneObject* CreateEntity(const std::string& name);
	void DestroyEntity(SceneObject entity);

	entt::registry registry;

	Shader* mainShader = nullptr;
	Material* mat = nullptr;
	Mesh* mesh = nullptr;

private:
	template<typename T>
	void OnComponentAdded(SceneObject entity, T& component);

	Shader* skyboxShader = nullptr;
	unsigned int texture = 0;
	Texture* tex = nullptr;
	Skybox* skyBox = nullptr;
	Camera* cam = nullptr;

	Scene* scene = nullptr;
	friend class SceneObject;
};
