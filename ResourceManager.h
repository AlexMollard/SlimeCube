#pragma once
#include "Material.h"
#include "Shader.h"
#include "Mesh.h"

class ResourceManager
{
public:
	static ResourceManager* GetInstance();

	ResourceManager(const ResourceManager&) = delete;
	ResourceManager(ResourceManager&&) = delete;
	ResourceManager& operator=(const ResourceManager&) = delete;
	ResourceManager& operator=(ResourceManager&&) = delete;
private:
	ResourceManager();
	~ResourceManager();

	static ResourceManager* instance;
};

