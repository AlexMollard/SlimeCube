#pragma once
#include "glew/glew.h"
#include "GLFW/glfw3.h"
#include "Resource.h"

class Texture : Resource
{
public:
	Texture(std::string name, std::string dir);
	Texture(std::string name, unsigned int id);
	Texture(std::string name, unsigned int* id);
	Texture(std::string dir, void* args);
	Texture(std::string name);
	Texture();
	~Texture();

	void SetUpTextureID();
	void load(std::string dir);
	void Bind() { glBindTexture(GL_TEXTURE_2D, textureID); };
	unsigned int GetID() { return textureID; };
	void SetID(unsigned int newID) { textureID = newID; };
	std::string GetName() { return name; };
	const char* GetNameC() { return name.c_str(); };

	static std::shared_ptr<Texture> Create(std::string const& dir);

protected:
	std::string name = "Default Name";
	uint32_t textureID;
};

