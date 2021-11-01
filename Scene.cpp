#include "Scene.h"
#include "Input.h"

Scene::Scene(Camera* cam)
{
	mainShader = new Shader("main", "Shaders/litVertex.shader", "Shaders/litFragment.shader");
	tex = new Texture("missingTex", "Images/missingTex.png");
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

	delete mesh;
	mesh = nullptr;

	delete mat;
	mat = nullptr;
}

void* Scene::Render(float deltaTime)
{
	// get matrix's uniform location and set matrix
	mainShader->Use();
	
	mainShader->setFloat("diffuseStrength", 1.0f);
	mainShader->setInt("diffuseTexture", 0);
	glActiveTexture(GL_TEXTURE0 + (int)0); // Texture unit 1
	tex->Bind();
	mainShader->setMat4("ProjectionView", cam->GetProjectionViewMatrix());

	for (int x = 0; x < 15; x++)
	for (int y = 0; y < 15; y++)
	{
		{
			glm::mat4 transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
			transform = glm::translate(transform, glm::vec3(x * 2.5f, 0.0f, y * 2.5f));
			transform = glm::rotate(transform, (float)glfwGetTime() - (x * y), glm::vec3(0.0f, 2.0f, 1.0f));

			mainShader->setMat4("Model", transform);

			mesh->draw();
		}
	}

	return (void*)texture;
}