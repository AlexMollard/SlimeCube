#include "pch.h"
#include "Window.h"
#include "Input.h"
#include "ImGuiLayer.h"
#include "Entity.h"
#include "entt.hpp"
#include "Components.h"

Input* Input::instance = nullptr;
ImGuiLayer* ImGuiLayer::instance = nullptr;

void ProcessMovement(float deltaTime, Camera* camera, Input* inputManager);
int main()
{
	Log::Init();
	Window* app = new Window(1920, 1080, (char*)"SlimeCore2D");
	Input* inputManager = Input::GetInstance();
	Camera* camera = new Camera(glm::perspective(glm::radians(45.0f), 1920.0f / 1080.0f, 0.1f, 100.0f));
	Input::GetInstance()->SetCamera(camera);
	Scene* scene = new Scene(camera);
	ImGuiLayer::SetScene(scene);

	while (!app->Window_shouldClose())
	{
		ProcessMovement(app->GetDeltaTime(), camera, inputManager);
		inputManager->Update();
		app->Update_Window();

		// Rendering
		ImGuiLayer::StartFrame();
		ImGuiLayer::Render(scene->Render(app->GetDeltaTime()));
		ImGuiLayer::EndFrame();
	}

	delete app;
	app = nullptr;

	delete camera;
	camera = nullptr;

	delete scene;
	scene = nullptr;

	ImGuiLayer::DeleteInstance();
	

	delete Input::GetInstance();

	return 0;
}

void ProcessMovement(float deltaTime, Camera* camera, Input* inputManager)
{
	if (inputManager->GetMouseDown(1))
	{
		glfwSetInputMode(inputManager->GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		if (inputManager->GetKeyPress(Keycode::W))
			camera->ProcessKeyboard(Camera_FORWARD, deltaTime);
		if (inputManager->GetKeyPress(Keycode::S))
			camera->ProcessKeyboard(Camera_BACKWARD, deltaTime);
		if (inputManager->GetKeyPress(Keycode::A))
			camera->ProcessKeyboard(Camera_LEFT, deltaTime);
		if (inputManager->GetKeyPress(Keycode::D))
			camera->ProcessKeyboard(Camera_RIGHT, deltaTime);
		if (inputManager->GetKeyPress(Keycode::SPACE))
			camera->ProcessKeyboard(Camera_UP, deltaTime);
		if (inputManager->GetKeyPress(Keycode::LEFT_CONTROL))
			camera->ProcessKeyboard(Camera_DOWN, deltaTime);

		if (inputManager->GetScroll() < 0)
			camera->ProcessKeyboard(Camera_BACKWARD, deltaTime * 4.0f);
		if (inputManager->GetScroll() > 0)
			camera->ProcessKeyboard(Camera_FORWARD, deltaTime * 4.0f);


		if (inputManager->GetKeyPress(Keycode::LEFT_SHIFT))
			camera->MovementSpeed = 20.5f;
		else
			camera->MovementSpeed = 10.5f;

		camera->ProcessMouseMovement(-inputManager->GetDeltaMouse().x, -inputManager->GetDeltaMouse().y);
	
		camera->UpdateProjectionViewMatrix();
	}
	else
	{
		glfwSetInputMode(inputManager->GetWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}