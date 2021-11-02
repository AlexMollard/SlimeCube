#pragma once
#include "Material.h"
#include "Shader.h"
#include "Mesh.h"

class SceneObject
{
public:
	SceneObject(Mesh* newMesh, Shader* newShader, Material* newMaterial);
	~SceneObject();

	// Mesh
	void SetMesh(Mesh* newMesh);
	Mesh* mesh();
	
	// Shader
	void SetShader(Shader* newShader);
	Shader* shader();
	
	// Material
	void SetMaterial(Material* newMaterial);
	Material* material();

	//Position
	void SetPosition(glm::vec3 newPosition);
	glm::vec3 GetPosition();

	// Rotation
	void SetRotation(glm::vec3 newRotation);
	glm::vec3 GetRotation();
	
	// Scale
	void SetScale(glm::vec3 newScale);
	glm::vec3 GetScale();

	// Transform
	glm::mat4 GetTransform();
private:
	void UpdateTransform();

	glm::vec3 position = glm::vec3(0.0f);
	glm::vec3 rotation = glm::vec3(0.0f);
	glm::vec3 scale = glm::vec3(0.0f);

	glm::mat4 transform = glm::mat4(1.0f);

	Mesh* meshRef = nullptr;
	Shader* shaderRef = nullptr;
	Material* materialRef = nullptr;
};

