#pragma once

#include "Camera.h"
#include "ResourceHub.h"
#include "Skybox.h"
#include "entt/entt.hpp"

class Entity;
class Material;
class Shader;
class Mesh;

class Scene
{
  public:
    Scene(Camera* cam, std::shared_ptr<ResourceHub> resHub);
    ~Scene() = default;

    void* Render(float deltaTime);

    std::shared_ptr<ResourceHub> GetResourceHub() const { return m_resourceHub; };
    entt::registry& GetRegistry() { return m_registry; };

    Entity CreateEntity(const std::string& name);
    void DestroyEntity(entt::entity entity);

    Camera* GetMainCamera();
    void AddPointLight(glm::vec3 pos);

  private:
    friend class Entity;

    void GenerateRenderTexture();
    void UpdateAndBindRenderTexture() const;

    entt::registry m_registry;

    std::shared_ptr<Shader> m_mainShader = nullptr;
    std::shared_ptr<Material> m_testingMaterial = nullptr;
    std::shared_ptr<Material> m_skyBoxMat = nullptr;
    std::shared_ptr<Mesh> m_testingMesh = nullptr;
    std::shared_ptr<Mesh> m_sceneMesh = nullptr;

    std::string m_name = "Main Scene";
    std::shared_ptr<Entity> m_firstObject;

    std::shared_ptr<Shader> m_skyboxShader = nullptr;
    unsigned int texture = 0;
    std::shared_ptr<Texture> m_texture = nullptr;
    std::shared_ptr<Skybox> m_skyBoxTex = nullptr;
    Camera* m_camera = nullptr;

    // Texture Stuff
    GLuint m_frameBufferName = 0;
    GLuint m_renderedTexture = 0;
    GLuint m_depthRenderBuffer = 0;
    GLenum m_drawBuffers[1] = {GL_COLOR_ATTACHMENT0};

    std::shared_ptr<ResourceHub> m_resourceHub = nullptr;
};
