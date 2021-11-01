#include "Window.h"
#include "Input.h"
#include "EditorGUI.h"
#include "Scene.h"
Input* Input::instance = 0;

void ProcessMovement(float deltaTime, Camera* camera, Input* inputManager);
int main()
{
	Window* app = new Window(1920, 1080, (char*)"SlimeCore2D");
	Input* inputManager = Input::GetInstance();
	Camera* camera = new Camera(glm::perspective(glm::radians(45.0f), 1920.0f / 1080.0f, 0.1f, 100.0f));
	Input::GetInstance()->SetCamera(camera);
	EditorGUI* gui = new EditorGUI();
	Scene* scene = new Scene(camera);

	while (!app->Window_shouldClose())
	{
		ProcessMovement(app->GetDeltaTime(), camera, inputManager);
		inputManager->Update();
		app->Update_Window();
		scene->Render(app->GetDeltaTime());
		//gui->Render();
	}

	delete app;
	app = nullptr;

	delete camera;
	camera = nullptr;

	delete scene;
	scene = nullptr;

	delete gui;
	gui = nullptr;

	delete Input::GetInstance();

	return 0;
}

void ProcessMovement(float deltaTime, Camera* camera, Input* inputManager)
{
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

	if (inputManager->GetKeyPress(Keycode::LEFT_SHIFT))
		camera->MovementSpeed = 20.5f;
	else
		camera->MovementSpeed = 10.5f;

	camera->ProcessMouseMovement(-inputManager->GetDeltaMouse().x, -inputManager->GetDeltaMouse().y);
	
	camera->UpdateProjectionViewMatrix();
}