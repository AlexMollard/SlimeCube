#pragma once
#include "Shader.h"
#include "Texture.h"
#include "Mesh.h"
#include "Material.h"
#include "Camera.h"

class Scene
{
public:
	Scene(Camera* cam);
	~Scene();

	void* Render(float deltaTime);

private:
	Shader* mainShader = nullptr;
	unsigned int VBO, VAO, EBO;
	unsigned int texture;
	Texture* tex = nullptr;
	Mesh* mesh = nullptr;
	Material* mat = nullptr;
	Camera* cam = nullptr;
};
