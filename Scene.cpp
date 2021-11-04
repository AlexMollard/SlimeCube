#include "Scene.h"
#include "Input.h"
#include "SceneObject.h"
#include "Components.h"

Scene::Scene(Camera* cam)
{
	mainShader = new Shader("main", "Shaders/litVertex.shader", "Shaders/litFragment.shader");
	skyboxShader = new Shader("skybox", "Shaders/SkyBoxVertex.shader", "Shaders/SkyBoxFragment.shader");
	tex = new Texture("missingTex", "Images/missingTex.png");
	skyBox = new Skybox("Images/SkyBox/");
	mesh = new Mesh("Cube");
	mesh->create();
	mat = new Material("main", tex);

	this->cam = cam;
}

Scene::~Scene()
{
	delete mainShader;
	mainShader = nullptr;

	delete tex;
	tex = nullptr;

	delete skyBox;
	skyBox = nullptr;

	delete mesh;
	mesh = nullptr;

	delete mat;
	mat = nullptr;

	delete skyboxShader;
	skyboxShader = nullptr;
}

void* Scene::Render(float deltaTime)
{
	skyboxShader->Use();
	skyboxShader->setMat4("ProjectionView", cam->GetProjectionViewMatrix());
	skyboxShader->setMat4("Model", glm::translate(glm::mat4(1.0f), cam->Position));

	glDepthMask(GL_FALSE);
	glActiveTexture(GL_TEXTURE0 + (int)0); // Texture unit 1
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyBox->GetID());
	skyBox->Bind();
	mesh->draw();
	glDepthMask(GL_TRUE);

	auto view = registry.view<TransformComponent, MeshComponent, ShaderComponent, MaterialComponent>();
	for (auto entity : view)
	{
		TransformComponent &transform = view.get<TransformComponent>(entity);
		MeshComponent &newMesh = view.get<MeshComponent>(entity);
		ShaderComponent &shader = view.get<ShaderComponent>(entity);
		MaterialComponent &material = view.get<MaterialComponent>(entity);
		
		shader.shader->Use();
		shader.shader->setFloat("diffuseStrength", 1.0f);
		shader.shader->setInt("diffuseTexture", 0);
		shader.shader->setMat4("ProjectionView", cam->GetProjectionViewMatrix());
		material.material->GetAlbedo()->Bind();

		shader.shader->setMat4("Model", transform.GetTransform());
		newMesh.mesh->draw();
	}

	return (void*)texture;
}

SceneObject* Scene::CreateEntity(const std::string& name)
{
	SceneObject entity = { registry.create(), this };
	entity.AddComponant<TransformComponent>();
	auto& tag = entity.AddComponant<TagComponent>();
	tag.Tag = name.empty() ? "Entity" : name;

	entity.AddComponant<MeshComponent>(mesh);
	entity.AddComponant<MaterialComponent>(mat);
	entity.AddComponant<ShaderComponent>(mainShader);


	return &entity;
}

void Scene::DestroyEntity(SceneObject entity)
{
	registry.destroy(entity);
}

template<typename T>
void Scene::OnComponentAdded(SceneObject entity, T& component)
{
	static_assert(false);
}