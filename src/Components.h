#pragma once

#include <utility>

#include "Camera.h"
#include "Material.h"
#include "Mesh.h"
#include "PointLight.h"
#include "Shader.h"

struct TagComponent
{
    std::string Tag;

    TagComponent() = default;

    TagComponent(const TagComponent&) = default;

    explicit TagComponent(std::string tag) : Tag(std::move(tag)) {}
};

struct TransformComponent
{
    std::string name = "Transform";

    glm::vec3 Translation = {0.0f, 0.0f, 0.0f};
    glm::vec3 Rotation = {0.0f, 0.0f, 0.0f};
    glm::vec3 Scale = {1.0f, 1.0f, 1.0f};

    TransformComponent() = default;

    TransformComponent(const TransformComponent&) = default;

    explicit TransformComponent(const glm::vec3& translation) : Translation(translation) {}

    void SetPosition(glm::vec3 newPos) { Translation = newPos; };

    glm::mat4 GetTransform() const
    {
        glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));

        return glm::translate(glm::mat4(1.0f), Translation) * rotation * glm::scale(glm::mat4(1.0f), Scale);
    }
};

struct MeshComponent
{
    std::string name = "Mesh";
    std::shared_ptr<Mesh> mesh = nullptr;

    MeshComponent() = default;

    MeshComponent(const MeshComponent&) = default;

    explicit MeshComponent(std::shared_ptr<Mesh> Mesh) : mesh(std::move(Mesh)) {}
};

struct ShaderComponent
{
    std::string name = "Shader";
    std::shared_ptr<Shader> shader = nullptr;

    ShaderComponent() = default;

    ShaderComponent(const ShaderComponent&) = default;

    explicit ShaderComponent(std::shared_ptr<Shader> Shader) : shader(std::move(Shader)) {}
};

struct MaterialComponent
{
    std::string name = "Material";
    std::shared_ptr<Material> material = nullptr;

    MaterialComponent() = default;

    MaterialComponent(const MaterialComponent&) = default;

    explicit MaterialComponent(std::shared_ptr<Material> Material) : material(std::move(Material)) {}
};

struct SkyBoxComponent
{
    Camera* m_camera = nullptr;

    SkyBoxComponent() = default;

    explicit SkyBoxComponent(Camera* camera) { m_camera = camera; };

    SkyBoxComponent(const SkyBoxComponent&) = default;
};

struct CameraComponent
{
    Camera* cam = nullptr;

    CameraComponent() = default;

    explicit CameraComponent(Camera* camera) { cam = camera; };

    CameraComponent(const CameraComponent&) = default;
};

struct PointLightComponent
{
    std::shared_ptr<PointLight> light = nullptr;

    PointLightComponent() = default;

    PointLightComponent(const PointLightComponent&) = default;

    explicit PointLightComponent(std::shared_ptr<PointLight> Light) : light(std::move(Light)) {}
};