#include "Camera.h"

#include "Input.h"

// Constructor with vectors
Camera::Camera(glm::mat4 projection, glm::vec3 position, glm::vec3 up, float yaw, float pitch)
    : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY)
{
    Projection = projection;
    Position = position;
    WorldUp = up;
    Yaw = yaw;
    Pitch = pitch;
    updateCameraVectors();
}

// Constructor with scalar values
Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
    : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY)
{
    Position = glm::vec3(posX, posY, posZ);
    WorldUp = glm::vec3(upX, upY, upZ);
    Yaw = yaw;
    Pitch = pitch;
    updateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix() const { return glm::lookAt(Position, Position + Front, Up); }

glm::mat4 Camera::GetProjectionMatrix() const { return Projection; }

void Camera::UpdateProjectionViewMatrix()
{
    float aspectRatio = (Input::GetAspectRatio() > 0 ? Input::GetAspectRatio() : 1.777f);
    Projection = glm::perspective(glm::radians(fov), aspectRatio, 0.1f, 1000.0f);
    ProjectionViewMat = Projection * GetViewMatrix();
}

glm::mat4 Camera::GetProjectionViewMatrix() const { return ProjectionViewMat; }

// Processes input received from any keyboard-like input system. Accepts input
// parameter in the form of camera defined ENUM (to abstract it from windowing
// systems)
void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
    float velocity = MovementSpeed * deltaTime;
    if (direction == Camera_FORWARD)
        Position += Front * velocity;
    if (direction == Camera_BACKWARD)
        Position -= Front * velocity;
    if (direction == Camera_LEFT)
        Position -= Right * velocity;
    if (direction == Camera_RIGHT)
        Position += Right * velocity;
    if (direction == Camera_UP)
        Position += Up * velocity;
    if (direction == Camera_DOWN)
        Position -= Up * velocity;
}

// Processes input received from a mouse input system. Expects the offset value
// in both the x and y direction.
void Camera::ProcessMouseMovement(float xOffset, float yOffset, bool constrainPitch)
{
    xOffset *= MouseSensitivity;
    yOffset *= MouseSensitivity;

    Yaw += xOffset;
    Pitch += yOffset;

    // Make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch) {
        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;
    }

    // Update Front, Right and Up Vectors using the updated Euler angles
    updateCameraVectors();
}

// Calculates the front vector from the Camera's (updated) Euler Angles
void Camera::updateCameraVectors()
{
    // Calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);
    // Also re-calculate the Right and Up vector
    Right = glm::normalize(glm::cross(Front, WorldUp));
    // Normalize the vectors, because their length gets
    // closer to 0 the more you look up or down which
    // results in slower movement.
    Up = glm::normalize(glm::cross(Right, Front));
}