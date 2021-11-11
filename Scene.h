#pragma once
#include "Skybox.h"
#include "Camera.h"
#include "entt.hpp"

class Entity;
class Material;
class Shader;
class Mesh;

class Scene
{
public:
	Scene(Camera* cam);
	~Scene();

	void* Render(float deltaTime);

	Entity CreateEntity(const std::string& name);
	void DestroyEntity(Entity entity);

	entt::registry registry;

	Shader* mainShader = nullptr;
	Material* mat = nullptr;
	Material* skyBoxMat = nullptr;
	Mesh* mesh = nullptr;

	std::string name = "Main Scene";
	Entity* firstObject;

	Camera* GetMainCamera();

private:
	void GenerateRenderTexture();
	void UpdateAndBindRenderTexture();
	Shader* skyboxShader = nullptr;
	unsigned int texture = 0;
	Texture* tex = nullptr;
	Skybox* skyBoxTex = nullptr;
	Camera* cam = nullptr;

	friend class Entity;

	// Texture Stuff
	GLuint FramebufferName = 0;
	GLuint renderedTexture;
	GLuint depthrenderbuffer;
	GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };


};
