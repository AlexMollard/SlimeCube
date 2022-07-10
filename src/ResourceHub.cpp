#include "ResourceHub.h"

ResourceHub::ResourceHub()
{
 textureManager = std::make_shared<ResourceManager<Texture>>();
 textureManager->Initialise("Texture Manager");

 materialManager = std::make_shared<ResourceManager<Material>>();
 materialManager->Initialise("Material Manager");

 meshManager = std::make_shared<ResourceManager<Mesh>>();
 meshManager->Initialise("Mesh Manager");

 shaderManager = std::make_shared<ResourceManager<Shader>>();
 shaderManager->Initialise("Shader Manager");
}
