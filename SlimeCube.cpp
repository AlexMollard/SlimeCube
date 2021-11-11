#include "pch.h"
#include "SlimeCube.h"

Input* Input::instance = nullptr;
ImGuiLayer* ImGuiLayer::instance = nullptr;

int main()
{
	Log::Init();
	Window* app = new Window(1920, 1080, (char*)"SlimeCore2D");
	SlimeCube core;

	while (!app->Window_shouldClose())
	{
		app->Update_Window();
		core.Update(app->GetDeltaTime());
	}

	delete app;
	app = nullptr;

	return 0;
}

SlimeCube::SlimeCube()
{
	inputManager = Input::GetInstance();
	camera = new Camera(glm::perspective(glm::radians(60.0f), 1920.0f / 1080.0f, 0.1f, 100.0f));
	Input::GetInstance()->SetCamera(camera);
	scene = new Scene(camera);
	ImGuiLayer::SetScene(scene);
}

SlimeCube::~SlimeCube()
{
	delete camera;
	camera = nullptr;

	delete scene;
	scene = nullptr;

	ImGuiLayer::DeleteInstance();

	delete Input::GetInstance();
}

void SlimeCube::Update(float deltaTime)
{
	ProcessMovement(deltaTime);
	inputManager->Update();

	// Rendering
	ImGuiLayer::StartFrame();
	ImGuiLayer::Render(scene->Render(deltaTime));
	ImGuiLayer::EndFrame();
}

void SlimeCube::ProcessMovement(float deltaTime)
{
	if (windowSize != Input::GetWindowSize())
	{
		windowSize = Input::GetWindowSize();
		camera->UpdateProjectionViewMatrix();
	}

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

		if (inputManager->GetKeyPress(Keycode::E))
			ImGuiLayer::SetGizmoState(ImGuizmo::OPERATION::TRANSLATE);
		
		if (inputManager->GetKeyPress(Keycode::R))
			ImGuiLayer::SetGizmoState(ImGuizmo::OPERATION::ROTATE);

		if (inputManager->GetKeyPress(Keycode::T))
			ImGuiLayer::SetGizmoState(ImGuizmo::OPERATION::SCALE);
	}
}


