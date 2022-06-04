#pragma once

#include "Skybox.h"
#include "Camera.h"
#include "entt/entt.hpp"
#include "ResourceHub.h"
#include <iostream>

class Entity;

class Material;

class Shader;

class Mesh;

class Scene {
public:
    Scene(Camera *cam, std::shared_ptr<ResourceHub> resHub);

    ~Scene();

    void *Render(float deltaTime);

    Entity CreateEntity(const std::string &name);

    void DestroyEntity(Entity entity);

    entt::registry registry;

    std::shared_ptr<Shader> mainShader = nullptr;
    std::shared_ptr<Material> mat = nullptr;
    std::shared_ptr<Material> skyBoxMat = nullptr;
    std::shared_ptr<Mesh> mesh = nullptr;
    std::shared_ptr<Mesh> sceneMesh = nullptr;

    std::string name = "Main Scene";
    std::shared_ptr<Entity> firstObject;

    Camera *GetMainCamera();

    std::shared_ptr<ResourceHub> GetResourceHub() { return resourceHub; };

    void AddPointLight(glm::vec3 pos);

private:
    void GenerateRenderTexture();

    void UpdateAndBindRenderTexture();

    std::shared_ptr<Shader> skyboxShader = nullptr;
    unsigned int texture = 0;
    std::shared_ptr<Texture> tex = nullptr;
    std::shared_ptr<Skybox> skyBoxTex = nullptr;
    Camera *cam = nullptr;

    friend class Entity;

    // Texture Stuff
    GLuint FramebufferName = 0;
    GLuint renderedTexture;
    GLuint depthrenderbuffer;
    GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};

    std::shared_ptr<ResourceHub> resourceHub;
};
