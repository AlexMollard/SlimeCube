#include "SceneObject.h"

SceneObject::SceneObject(Mesh* newMesh, Shader* newShader, Material* newMaterial)
{
    meshRef = newMesh;
    shaderRef = newShader;
    materialRef = newMaterial;
}

SceneObject::~SceneObject()
{
}


void SceneObject::SetMesh(Mesh* newMesh)
{
    meshRef = newMesh;
}

Mesh* SceneObject::mesh()
{
    return meshRef;
}

void SceneObject::SetShader(Shader* newShader)
{
    shaderRef = newShader;
}

Shader* SceneObject::shader()
{
    return shaderRef;
}

void SceneObject::SetMaterial(Material* newMaterial)
{
    materialRef = newMaterial;
}

Material* SceneObject::material()
{
    return materialRef;
}

void SceneObject::SetPosition(glm::vec3 newPosition)
{
    position = newPosition;
    UpdateTransform();
}

glm::vec3 SceneObject::GetPosition()
{
    return position;
}

void SceneObject::SetRotation(glm::vec3 newRotation)
{
    rotation = newRotation;
    UpdateTransform();
}

glm::vec3 SceneObject::GetRotation()
{
    return rotation;
}

void SceneObject::SetScale(glm::vec3 newScale)
{
    scale = newScale;
}

glm::vec3 SceneObject::GetScale()
{
    return scale;
}

glm::mat4 SceneObject::GetTransform()
{
    return transform;
}

void SceneObject::UpdateTransform()
{
    transform = glm::translate(glm::mat4(1.0f), position);
    transform = glm::rotate(transform, 1.0f, rotation);
}
