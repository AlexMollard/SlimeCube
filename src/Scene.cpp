#include "Scene.h"

#include "Components.h"
#include "Directories.h"
#include "Entity.h"
#include "ImGuiLayer.h"
#include "Renderer.h"
#include "imgui_internal.h"

Scene::Scene(Camera *cam, std::shared_ptr<ResourceHub> resHub)
{
 m_resourceHub = std::move(resHub);
 m_mainShader = m_resourceHub->GetShaderManager()->Load(
     Dir::FromAsset("/Shaders/litVertex.shader"), Dir::FromAsset("/Shaders/litFragment.shader"));

 m_skyboxShader = m_resourceHub->GetShaderManager()->Load(
     Dir::FromAsset("/Shaders/SkyBoxVertex.shader"), Dir::FromAsset("/Shaders/SkyBoxFragment.shader"));

 m_texture = m_resourceHub->GetTextureManager()->Load(Dir::FromAsset("/Images/missingTex.png"));

 auto normalTex = m_resourceHub->GetTextureManager()->Load(Dir::FromAsset("/Images/Normal/Metal.jpg"));
 m_testingMesh = m_resourceHub->GetMeshManager()->Load(std::make_shared<Mesh>("Cube"));
 m_testingMesh->Create(Primitives::Cube);

 m_testingMaterial = m_resourceHub->GetMaterialManager()->Load(std::make_shared<Material>("Basic Material", m_texture));
 m_testingMaterial->SetAmbientMap(m_texture);
 m_testingMaterial->SetNormalMap(normalTex);

 this->m_camera = cam;
 cam->Position = glm::vec3(0.0f, 7.0f, 10.0f);

 auto testingMesh = CreateEntity("Testing Mesh");
 testingMesh.GetComponent<MeshComponent>().mesh = m_resourceHub->GetMeshManager()->Load(std::make_shared<Mesh>("Testing Mesh"));
 testingMesh.GetComponent<MeshComponent>().mesh->Load(Dir::FromAsset("/Models/test/cornell_box_multimaterial.obj").c_str());
 testingMesh.GetComponent<TransformComponent>().SetPosition(glm::vec3(0.0f, 2.0f, 0.0f));

 // Move most of this into the Skybox class
 Entity skyBox = CreateEntity("SkyBox");
 m_skyBoxTex = std::make_shared<Skybox>(Dir::FromAsset("/Images/SkyBox/"));
 m_skyBoxMat = std::make_shared<Material>("Skybox Material", m_skyBoxTex);
 skyBox.AddComponent<SkyBoxComponent>(cam);
 skyBox.GetComponent<MaterialComponent>().material = m_skyBoxMat;
 skyBox.GetComponent<ShaderComponent>().shader = m_skyboxShader;

 int cubeCount = 1;
 for (int i = -cubeCount; i < cubeCount; i++) {
  for (int x = -cubeCount; x < cubeCount; x++) {
   CreateEntity("Testing Cube").GetComponent<TransformComponent>().SetPosition(glm::vec3((i * 3.0f) + 1.5f, 0.0f, (x * 3.0f)));
  }
 }
 AddPointLight(glm::vec3(0.0f));
 cam->UpdateProjectionViewMatrix();

 GenerateRenderTexture();
}

void *Scene::Render(float deltaTime)
{
 UpdateAndBindRenderTexture();

 // Draw SkyBox
 auto view = m_registry.view<SkyBoxComponent>();
 for (auto entityID : view) {
  Entity entity{entityID, this};
  Renderer::DrawEntity(entity);
 }

 // Draw all but Skybox
 m_registry.each([&](auto entityID) {
  Entity entity{entityID, this};
  if (entity.HasComponent<TagComponent>() && !entity.HasComponent<SkyBoxComponent>()) {
   Renderer::DrawEntity(entity);
  }
 });
 glBindFramebuffer(GL_FRAMEBUFFER, 0);
 return (void *)m_renderedTexture;
}

Entity Scene::CreateEntity(const std::string &name)
{
 Entity entity = Entity{m_registry.create(), this};
 entity.AddComponent<TransformComponent>();
 auto &tag = entity.AddComponent<TagComponent>();
 tag.Tag = name.empty() ? "Entity" : name;

 entity.AddComponent<MeshComponent>(m_testingMesh);
 entity.AddComponent<MaterialComponent>(m_testingMaterial);
 entity.AddComponent<ShaderComponent>(m_mainShader);
 entity.AddComponent<CameraComponent>(m_camera);

 return entity;
}

void Scene::DestroyEntity(entt::entity entity) { m_registry.destroy(entity); }

Camera *Scene::GetMainCamera() { return m_camera; }

void Scene::AddPointLight(glm::vec3 pos)
{
 auto light = CreateEntity("PointLight");
 light.AddComponent<PointLightComponent>(std::make_shared<PointLight>());
 light.GetComponent<TransformComponent>().SetPosition(pos);
 light.GetComponent<TransformComponent>().Scale = glm::vec3(0.1f);
 light.GetComponent<PointLightComponent>().light->SetStrength(20.0f);
 Renderer::AddPointLight(std::make_shared<Entity>(light));
}

void Scene::GenerateRenderTexture()
{
 // The frame buffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
 glGenFramebuffers(1, &m_frameBufferName);
 glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferName);

 // The texture we're going to render to
 glGenTextures(1, &m_renderedTexture);

 // "Bind" the newly created texture : all future texture functions will modify this texture
 glBindTexture(GL_TEXTURE_2D, m_renderedTexture);

 // Give an empty image to OpenGL ( the last "0" )
 glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1920, 1080, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

 // Poor filtering. Needed !
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

 // The depth buffer
 glGenRenderbuffers(1, &m_depthRenderBuffer);
 glBindRenderbuffer(GL_RENDERBUFFER, m_depthRenderBuffer);
 glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1920, 1080);
 glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthRenderBuffer);

 // Set "m_renderedTexture" as our colour attachment #0
 glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_renderedTexture, 0);

 // Set the list of draw buffers.
 glDrawBuffers(1, m_drawBuffers);  // "1" is the size of m_drawBuffers

 // Always check that our frame buffer is ok
 if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) Log::Error("THE TEXTURE IS COOKED");
}

void Scene::UpdateAndBindRenderTexture() const
{
 ImVec2 renderSize = ImGuiLayer::GetViewPortSize();

 // "Bind" the newly created texture : all future texture functions will modify this texture
 glBindTexture(GL_TEXTURE_2D, m_renderedTexture);

 // Give an empty image to OpenGL ( the last "0" )
 glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, renderSize.x, renderSize.y, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

 glBindRenderbuffer(GL_RENDERBUFFER, m_depthRenderBuffer);
 glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, renderSize.x, renderSize.y);
 glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthRenderBuffer);

 // Render to our frame buffer
 glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferName);
 glViewport(0, 0, renderSize.x,
     renderSize.y);  // Render on the whole frame buffer, complete from the lower left corner to the upper right
 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
