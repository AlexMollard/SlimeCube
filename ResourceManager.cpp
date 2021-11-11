#include "pch.h"
#include "ResourceManager.h"

ResourceManager* ResourceManager::instance = nullptr;

ResourceManager* ResourceManager::GetInstance()
{
	if (!instance)
	{
		instance = new ResourceManager();
	}

	return instance;
}

ResourceManager::ResourceManager()
{

}

ResourceManager::~ResourceManager()
{

}
