#include "Scene.h"
#include "Input.h"
#include "SceneObject.h"
#include "Components.h"
#include <memory>

Scene::Scene(Camera* cam)
{
	scene = this;
	mainShader = new Shader("main", "Shaders/litVertex.shader", "Shaders/litFragment.shader");
	skyboxShader = new Shader("skybox", "Shaders/SkyBoxVertex.shader", "Shaders/SkyBoxFragment.shader");
	tex = new Texture("missingTex", "Images/missingTex.png");
	skyBoxTex = new Skybox("Images/SkyBox/");
	mesh = new Mesh("Cube");
	mesh->create(Primitives::Cube);
	mat = new Material("main", tex);
	skyBoxMat = new Material("skyboxmaterial", skyBoxTex);

	this->cam = cam;
	cam->Position = glm::vec3(0.0f,5.0f,15.0f);
	
	SceneObject skyBox = CreateEntity("SkyBox");
	skyBox.AddComponent<SkyBoxComponent>(cam);
	skyBox.GetComponent<MaterialComponent>().material = skyBoxMat;
	skyBox.GetComponent<ShaderComponent>().shader = skyboxShader;

	CreateEntity("Testing Cube");

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
	auto view = registry.view<SkyBoxComponent>();
	for (auto entityID : view)
	{
		SceneObject entity{ entityID , this };
		RenderObject(entity);
	}

	registry.each([&](auto entityID)
	{
		SceneObject entity{ entityID , this };
		if (entity.HasComponent<TagComponent>() && !entity.HasComponent<SkyBoxComponent>())
		{
			RenderObject(entity);
		}
	});
}

SceneObject Scene::CreateEntity(const std::string& name)
{
	SceneObject entity = SceneObject{ registry.create(), this };
	entity.AddComponent<TransformComponent>();
	auto& tag = entity.AddComponent<TagComponent>();
	tag.Tag = name.empty() ? "Entity" : name;

	entity.AddComponent<MeshComponent>(mesh);
	entity.AddComponent<MaterialComponent>(mat);
	entity.AddComponent<ShaderComponent>(mainShader);

	return entity;
}

void Scene::DestroyEntity(SceneObject entity)
{
	registry.destroy(entity);
}

void Scene::RenderObject(SceneObject entity)
{
	TransformComponent& transform = entity.GetComponent<TransformComponent>();
	MeshComponent& newMesh = entity.GetComponent<MeshComponent>();
	ShaderComponent& shader = entity.GetComponent<ShaderComponent>();
	MaterialComponent& material = entity.GetComponent<MaterialComponent>();

	shader.shader->Use();
	shader.shader->setFloat("diffuseStrength", 1.0f);
	shader.shader->setInt("diffuseTexture", 0);
	shader.shader->setMat4("ProjectionView", cam->GetProjectionViewMatrix());
	material.material->GetAlbedo()->Bind();


	if (entity.HasComponent<SkyBoxComponent>())
	{
		glDepthMask(GL_FALSE);
		glCullFace(GL_FRONT);
		SkyBoxComponent& camera = entity.GetComponent<SkyBoxComponent>();
		transform.SetPosition(camera.cam->Position);
		shader.shader->setMat4("Model", transform.GetTransform());
		newMesh.mesh->draw();
		glCullFace(GL_BACK);
		glDepthMask(GL_TRUE);
		return;
	}

	shader.shader->setMat4("Model", transform.GetTransform());
	newMesh.mesh->draw();
}

template<typename T>
void Scene::OnComponentAdded(SceneObject entity, T& component)
{
	//std::cout << "Component Added to : " << (int)entity.entityHandle << std::endl;
	//static_assert(false);
}