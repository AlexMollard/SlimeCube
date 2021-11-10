#pragma once
#include "glm.hpp"
#include "ext.hpp"
#include <GLFW/glfw3.h>

enum Camera_Movement
{
	Camera_FORWARD,
	Camera_BACKWARD,
	Camera_LEFT,
	Camera_RIGHT,
	Camera_UP,
	Camera_DOWN
};

// Default camera values
const float YAW = -90.0f;
const float PITCH = -15.0f;
const float SPEED = 10.5f;
const float SENSITIVITY = 0.2f;
const float ZOOM = 45.0f;

class Camera
{
public:
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;

	glm::mat4 Projection = glm::mat4(1);
	glm::mat4 ProjectionViewMat = glm::mat4(1);

	float Yaw;
	float Pitch;

	float MovementSpeed;
	float MouseSensitivity = 100.0f;
	float Zoom = 0.0f;

	// Constructor with vectors
	Camera(glm::mat4 projection, glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);

	// Constructor with scalar values
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

	// Returns the view matrix calculated using Euler Angles and the LookAt Matrix
	glm::mat4 GetViewMatrix();
	glm::mat4 GetViewWithoutPosition();
	glm::mat4 GetProjectionMatrix();

	void UpdateProjectionViewMatrix();
	glm::mat4 GetProjectionViewMatrix();

	// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
	void ProcessKeyboard(Camera_Movement direction, float deltaTime);

	// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
	void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);

	// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
	void ProcessMouseScroll(float yoffset);

	bool LockMouse(GLFWwindow* window, bool down);
	bool GetMouseLock();

private:
	bool mouseLock = false;
	// Calculates the front vector from the Camera's (updated) Euler Angles
	void updateCameraVectors();
};