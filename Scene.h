#pragma once
#include "Skybox.h"
#include "Camera.h"
#include "SceneObject.h"

class Scene
{
public:
	Scene(Camera* cam);
	~Scene();

	void* Render(float deltaTime);

private:
	SceneObject* obj = nullptr;
	Shader* mainShader = nullptr;
	Shader* skyboxShader = nullptr;
	unsigned int VBO, VAO, EBO;
	unsigned int texture;
	Texture* tex = nullptr;
	Skybox* skyBox = nullptr;
	Mesh* mesh = nullptr;
	Material* mat = nullptr;
	Camera* cam = nullptr;
};
