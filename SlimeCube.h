#pragma once
#include "Window.h"
#include "ImGuiLayer.h"
#include "Entity.h"
#include "entt.hpp"
#include "Components.h"

class SlimeCube 
{
public:
	SlimeCube();
	~SlimeCube();

	void Update(float deltaTime);
private:
	void ProcessMovement(float deltaTime); 

	Input* inputManager = nullptr;
	Camera* camera = nullptr;
	Scene* scene = nullptr;

	glm::vec2 windowSize = glm::vec2(1920, 1080);
};