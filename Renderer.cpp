#include "pch.h"
#include "Renderer.h"
#include "Components.h"

Renderer* Renderer::instance = nullptr;

Renderer* Renderer::GetInstance()
{
	if (!instance)
	{
		instance = new Renderer();
		Log::Info(__func__ + std::string(" Instance Created"));
	}

	return instance;
}
// This function should add to a buffer and draw when it is full or at the end of the frame
void Renderer::DrawEntity(Entity entity)
{
	auto& transform = entity.GetComponent<TransformComponent>();
	auto& newMesh = entity.GetComponent<MeshComponent>();
	auto& shader = entity.GetComponent<ShaderComponent>();
	auto& material = entity.GetComponent<MaterialComponent>();
	auto& camera = entity.GetComponent<CameraComponent>();

	shader.shader->Use();
	shader.shader->setFloat("diffuseStrength", 1.0f);
	shader.shader->setInt("diffuseTexture", 0);
	shader.shader->setMat4("ProjectionView", camera.cam->GetProjectionViewMatrix());
	material.material->GetAlbedo()->Bind();


	if (entity.HasComponent<SkyBoxComponent>())
	{
		glDepthMask(GL_FALSE);
		glCullFace(GL_FRONT);
		auto& camera = entity.GetComponent<SkyBoxComponent>();
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