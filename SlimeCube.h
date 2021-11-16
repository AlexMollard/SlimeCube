#pragma once
#include "Window.h"
#include "ImGuiLayer.h"
#include "Entity.h"
#include "entt.hpp"
#include "Components.h"
#include "ResourceManager.h"

class SlimeCube 
{
public:
	SlimeCube();
	~SlimeCube();

	void Update(float deltaTime);
private:
	void ProcessMovement(float deltaTime); 

	Input* inputManager = nullptr;
	std::unique_ptr<Camera> camera;
	std::unique_ptr<Scene> scene;
	ResourceManager<Texture> TextureManager;

	glm::vec2 windowSize = glm::vec2(1920, 1080);
};