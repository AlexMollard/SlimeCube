#include "pch.h"
#include "Scene.h"
#include "Input.h"
#include "Entity.h"
#include "Components.h"
#include "Renderer.h"
#include "imgui_internal.h"
#include "ImGuiLayer.h"

Scene::Scene(Camera* cam)
{
	mainShader = new Shader("main", "Assets/Shaders/litVertex.shader", "Assets/Shaders/litFragment.shader");
	skyboxShader = new Shader("sky box", "Assets/Shaders/SkyBoxVertex.shader", "Assets/Shaders/SkyBoxFragment.shader");
	tex = new Texture("missingTex", "Assets/Images/missingTex.png");
	mesh = new Mesh("Cube");
	mesh->create(Primitives::Cube);
	mat = new Material("main", tex);

	this->cam = cam;
	cam->Position = glm::vec3(0.0f,5.0f,15.0f);
	
	// Move most of this into the Skybox class
	Entity skyBox = CreateEntity("SkyBox");
	skyBoxTex = new Skybox("Assets/Images/SkyBox/");
	skyBoxMat = new Material("Skybox Material", skyBoxTex);
	skyBox.AddComponent<SkyBoxComponent>(cam);
	skyBox.GetComponent<MaterialComponent>().material = skyBoxMat;
	skyBox.GetComponent<ShaderComponent>().shader = skyboxShader;
	
	int cubeCount = 4;
	for (int i = -cubeCount; i < cubeCount; i++)
	{
		CreateEntity("Testing Cube").GetComponent<TransformComponent>().SetPosition(glm::vec3((i * 3.0f) + 1.5f,0.0f,0.0f));
	}
	
	cam->UpdateProjectionViewMatrix();

	GenerateRenderTexture();
}

Scene::~Scene()
{
	delete mainShader;
	mainShader = nullptr;

	delete tex;
	tex = nullptr;

	delete skyBoxMat;
	skyBoxMat = nullptr;

	delete skyBoxTex;
	skyBoxTex = nullptr;

	delete mesh;
	mesh = nullptr;

	delete mat;
	mat = nullptr;

	delete skyboxShader;
	skyboxShader = nullptr;
}

void* Scene::Render(float deltaTime)
{
	UpdateAndBindRenderTexture();

	// Draw SkyBox
	auto view = registry.view<SkyBoxComponent>();
	for (auto entityID : view)
	{
		Entity entity{ entityID , this };
		Renderer::DrawEntity(entity);
	}

	// Draw all but Skybox
	registry.each([&](auto entityID)
	{
		Entity entity{ entityID , this };
		if (entity.HasComponent<TagComponent>() && !entity.HasComponent<SkyBoxComponent>())
		{
			Renderer::DrawEntity(entity);
		}
	});
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return (void*)renderedTexture;
}

Entity Scene::CreateEntity(const std::string& name)
{
	Entity entity = Entity{ registry.create(), this };
	entity.AddComponent<TransformComponent>();
	auto& tag = entity.AddComponent<TagComponent>();
	tag.Tag = name.empty() ? "Entity" : name;

	entity.AddComponent<MeshComponent>(mesh);
	entity.AddComponent<MaterialComponent>(mat);
	entity.AddComponent<ShaderComponent>(mainShader);
	entity.AddComponent<CameraComponent>(cam);

	return entity;
}

void Scene::DestroyEntity(Entity entity)
{
	registry.destroy(entity);
}

Camera* Scene::GetMainCamera()
{
	return cam;
}

void Scene::GenerateRenderTexture()
{
	// The frame buffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
	glGenFramebuffers(1, &FramebufferName);
	glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);

	// The texture we're going to render to
	glGenTextures(1, &renderedTexture);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, renderedTexture);

	// Give an empty image to OpenGL ( the last "0" )
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1920, 1080, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

	// Poor filtering. Needed !
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// The depth buffer
	glGenRenderbuffers(1, &depthrenderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1920, 1080);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);

	// Set "renderedTexture" as our colour attachment #0
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderedTexture, 0);

	// Set the list of draw buffers.
	glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

	// Always check that our frame buffer is ok
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		Log::Error("THE TEXTURE IS COOKED");
}

void Scene::UpdateAndBindRenderTexture()
{
	ImVec2 renderSize = ImGuiLayer::GetViewPortSize();

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, renderedTexture);

	// Give an empty image to OpenGL ( the last "0" )
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, renderSize.x, renderSize.y, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

	glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, renderSize.x, renderSize.y);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);

	// Render to our frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
	glViewport(0, 0, renderSize.x, renderSize.y); // Render on the whole frame buffer, complete from the lower left corner to the upper right
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
