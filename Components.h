#pragma once
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include "gtx/quaternion.hpp"

#include "Mesh.h"
#include "Shader.h"
#include "Material.h"
#include "Camera.h"

struct TagComponent
{
	std::string Tag;

	TagComponent() = default;
	TagComponent(const TagComponent&) = default;
	TagComponent(const std::string& tag)
		: Tag(tag) {}
};

struct TransformComponent
{
	std::string name = "Transform";

	glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
	glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
	glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

	TransformComponent() = default;
	TransformComponent(const TransformComponent&) = default;
	TransformComponent(const glm::vec3& translation)
		: Translation(translation) {}

	void SetPosition(glm::vec3 newPos) { Translation = newPos; };

	glm::mat4 GetTransform() const
	{
		glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));

		return glm::translate(glm::mat4(1.0f), Translation)
			* rotation
			* glm::scale(glm::mat4(1.0f), Scale);
	}
};

struct MeshComponent
{
	std::string name = "Mesh";
	std::shared_ptr < Mesh> mesh = nullptr;

	MeshComponent() = default;
	MeshComponent(const MeshComponent&) = default;
	MeshComponent(std::shared_ptr < Mesh> Mesh)
		: mesh(Mesh) {}
};

struct ShaderComponent
{
	std::string name = "Shader";
	std::shared_ptr < Shader> shader = nullptr;

	ShaderComponent() = default;
	ShaderComponent(const ShaderComponent&) = default;
	ShaderComponent(std::shared_ptr < Shader> Shader)
		: shader(Shader) {}
};

struct MaterialComponent
{
	std::string name = "Material";
	std::shared_ptr<Material> material = nullptr;

	MaterialComponent() = default;
	MaterialComponent(const MaterialComponent&) = default;
	MaterialComponent(std::shared_ptr < Material> Material)
		: material(Material) {}
};

struct SkyBoxComponent
{
	Camera* cam;

	SkyBoxComponent() = default;
	SkyBoxComponent(Camera* camera) { cam = camera; };
	SkyBoxComponent(const SkyBoxComponent&) = default;
};

struct CameraComponent
{
	Camera* cam;
	bool Primary = true;
	bool FixedAspectRatio = false;

	CameraComponent() = default;
	CameraComponent(Camera* camera) { cam = camera; };
	CameraComponent(const CameraComponent&) = default;
};

template<typename... Component>
struct ComponentGroup {};
using AllComponents = ComponentGroup
<
	TransformComponent,
	MeshComponent,
	ShaderComponent,
	CameraComponent,
	MaterialComponent
>;