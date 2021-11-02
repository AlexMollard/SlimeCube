#include "Scene.h"
#include "Input.h"

Scene::Scene(Camera* cam)
{
	mainShader = new Shader("main", "Shaders/litVertex.shader", "Shaders/litFragment.shader");
	skyboxShader = new Shader("skybox", "Shaders/SkyBoxVertex.shader", "Shaders/SkyBoxFragment.shader");
	tex = new Texture("missingTex", "Images/missingTex.png");
	skyBox = new Skybox("Images/SkyBox/");
	mesh = new Mesh("Cube");
	mesh->create();
	mat = new Material("main", tex);
	obj = new SceneObject(mesh,mainShader,mat);
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

	delete obj;
	obj = nullptr;
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

	obj->SetShader(mainShader);
	obj->shader()->Use();
	// get matrix's uniform location and set matrix
	obj->shader()->setFloat("diffuseStrength", 1.0f);
	obj->shader()->setInt("diffuseTexture", 0);
	obj->shader()->setMat4("ProjectionView", cam->GetProjectionViewMatrix());

	obj->material()->GetAlbedo()->Bind();
	for (int x = 0; x < 15; x++)
	for (int y = 0; y < 15; y++)
	{
		{
			if (x > 5)
				obj->material()->GetAlbedo()->SetID(skyBox->GetID());
			else
				obj->material()->GetAlbedo()->SetID(tex->GetID());

			obj->SetPosition( glm::vec3(x * 2.5f, 0.0f, y * 2.5f));
			obj->SetRotation(glm::vec3(0.0f, 2.0f * (float)glfwGetTime() + (x * y), 1.0f * (float)glfwGetTime() - (x * y)));

			obj->shader()->setMat4("Model", obj->GetTransform());

			obj->mesh()->draw();
		}
	}



	return (void*)texture;
}