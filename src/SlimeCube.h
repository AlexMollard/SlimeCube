#pragma once

#include "Components.h"
#include "Entity.h"
#include "ImGuiLayer.h"
#include "Input.h"
#include "Renderer.h"
#include "ResourceHub.h"
#include "Window.h"
#include "entt/entt.hpp"

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
    std::shared_ptr<ResourceHub> resourceHub;

    glm::vec2 windowSize = glm::vec2(1920, 1080);
};