#include "pch.h"
#include "Scene.h"
#include "Input.h"
#include "Entity.h"
#include "Components.h"
#include "Renderer.h"

Scene::Scene(Camera* cam)
{
	scene = this;
	mainShader = new Shader("main", "Shaders/litVertex.shader", "Shaders/litFragment.shader");
	skyboxShader = new Shader("sky box", "Shaders/SkyBoxVertex.shader", "Shaders/SkyBoxFragment.shader");
	tex = new Texture("missingTex", "Images/missingTex.png");
	mesh = new Mesh("Cube");
	mesh->create(Primitives::Cube);
	mat = new Material("main", tex);

	this->cam = cam;
	cam->Position = glm::vec3(0.0f,5.0f,15.0f);
	
	// Move most of this into the Skybox class
	Entity skyBox = CreateEntity("SkyBox");
	skyBoxTex = new Skybox("Images/SkyBox/");
	skyBoxMat = new Material("skyboxmaterial", skyBoxTex);
	skyBox.AddComponent<SkyBoxComponent>(cam);
	skyBox.GetComponent<MaterialComponent>().material = skyBoxMat;
	skyBox.GetComponent<ShaderComponent>().shader = skyboxShader;
	
	int cubeCount = 10;
	for (int i = 0; i < cubeCount; i++)
	{
		CreateEntity("Testing Cube").GetComponent<TransformComponent>().SetPosition(glm::vec3((i * 3.0f) - (cubeCount * 1.5f),0.0f,0.0f));
	}
	
	cam->UpdateProjectionViewMatrix();
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

void Scene::Render(float deltaTime)
{
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