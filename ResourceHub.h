#pragma once
#include "ResourceManager.h"
class ResourceHub
{
public:
	ResourceHub();
	~ResourceHub() = default;

	// Getters
	std::shared_ptr<ResourceManager<Texture>> GetTextureManager() { return textureManager; };
	std::shared_ptr<ResourceManager<Material>> GetMaterialManager() { return materialManager; };
	std::shared_ptr<ResourceManager<Mesh>> GetMeshManager() { return meshManager; };
	std::shared_ptr<ResourceManager<Shader>> GetShaderManager() { return shaderManager; };

private:
	std::shared_ptr<ResourceManager<Texture>> textureManager;
	std::shared_ptr<ResourceManager<Material>> materialManager;
	std::shared_ptr<ResourceManager<Mesh>> meshManager;
	std::shared_ptr<ResourceManager<Shader>> shaderManager;

};

