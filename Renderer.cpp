#include "pch.h"
#include "Renderer.h"
#include "Components.h"




std::shared_ptr<Renderer> Renderer::GetInstance()
{
	if (!instance)
	{
		instance = std::make_shared<Renderer>();
		Log::Info(__func__ + std::string(" Instance Created"));
	}

	return instance;
}

void Renderer::UpdateLights(std::shared_ptr<Shader> shader)
{
	// Spot Lights

	shader->setInt("pointLightTotal", 1);
	for (int i = 0; i < 1; i++)
	{
		shader->setVec3("pointLights[" + std::to_string(i) + "].position", GetInstance()->pointLights[i]->pos);
		shader->setVec3("pointLights[" + std::to_string(i) + "].ambient", GetInstance()->pointLights[i]->GetAmbient());
		shader->setVec3("pointLights[" + std::to_string(i) + "].albedo", GetInstance()->pointLights[i]->GetAlbedo());
		shader->setVec3("pointLights[" + std::to_string(i) + "].specular", GetInstance()->pointLights[i]->GetSpecular());

		shader->setFloat("pointLights[" + std::to_string(i) + "].strength", GetInstance()->pointLights[i]->GetStrength());

		shader->setFloat("pointLights[" + std::to_string(i) + "].constant", GetInstance()->pointLights[i]->GetConstant());
		shader->setFloat("pointLights[" + std::to_string(i) + "].linear", GetInstance()->pointLights[i]->GetLinear());
		shader->setFloat("pointLights[" + std::to_string(i) + "].quadratic", GetInstance()->pointLights[i]->GetQuadratic());
	}

	shader->setVec3("dirLight.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
	shader->setVec3("dirLight.ambient", glm::vec3(1.0, 0.75, 0.5));
	shader->setVec3("dirLight.albedo", glm::vec3(1.0, 0.75, 0.5));
	shader->setVec3("dirLight.specular", glm::vec3(1.0f));
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
	GetInstance()->BindTexture(shader.shader, TEXTURETYPE::Albedo, material.material->GetAlbedo());


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

	UpdateLights(shader.shader);

	shader.shader->setFloat("diffuseStrength", material.material->GetAlbedoStrength());
	shader.shader->setFloat("specularStrength", material.material->GetSpecularStrength());
	shader.shader->setFloat("normalStrength", material.material->GetNormalStrength());
	shader.shader->setFloat("ambientStrength", material.material->GetAmbientStrength());
	shader.shader->setFloat("roughStrength", material.material->GetRoughStrength());
	shader.shader->setFloat("displacementStrength", material.material->GetDisplacementStrength());

	GetInstance()->BindTexture(shader.shader, TEXTURETYPE::Specular, material.material->GetSpecMap());
	GetInstance()->BindTexture(shader.shader, TEXTURETYPE::Normal, material.material->GetNormalMap());
	GetInstance()->BindTexture(shader.shader, TEXTURETYPE::Ambient, material.material->GetAmbientMap());
	GetInstance()->BindTexture(shader.shader, TEXTURETYPE::Rough, material.material->GetRoughMap());
	GetInstance()->BindTexture(shader.shader, TEXTURETYPE::Displacement, material.material->GetDisplacementMap());


	shader.shader->setMat4("Model", transform.GetTransform());
	newMesh.mesh->draw();
}

void Renderer::BindTexture(std::shared_ptr<Shader> shader, TEXTURETYPE texType, std::shared_ptr<Texture> texture)
{
	if (texture != nullptr)
	{
		if (texture != currentTexture[(int)texType])
		{
			SetIntTexture(shader, texType);
			currentTexture[(int)texType] = texture;
			glActiveTexture(GL_TEXTURE0 + (int)texType); // Texture unit 1
			texture->Bind();
		}
	}
	else
	{
		SetIntTexture(shader, texType);
		currentTexture[(int)texType] = texture;
		glActiveTexture(GL_TEXTURE0 + (int)texType); // Texture unit 1
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void Renderer::SetIntTexture(std::shared_ptr<Shader> shader, TEXTURETYPE texType)
{
	switch (texType)
	{
	case TEXTURETYPE::Albedo:
		shader->setInt("diffuseTexture", (int)texType);
		break;
	case TEXTURETYPE::Specular:
		shader->setInt("specularTexture", (int)texType);
		break;
	case TEXTURETYPE::Normal:
		shader->setInt("normalTexture", (int)texType);
		break;
	case TEXTURETYPE::Ambient:
		shader->setInt("ambientTexture", (int)texType);
		break;
	case TEXTURETYPE::Rough:
		shader->setInt("roughTexture", (int)texType);
		break;
	case TEXTURETYPE::Displacement:
		shader->setInt("displacementTexture", (int)texType);
		break;
	default:
		break;
	}
}