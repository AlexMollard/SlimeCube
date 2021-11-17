#pragma once
#include "Material.h"
#include "Shader.h"
#include "Mesh.h"

template <class T>
class ResourceManager
{
private:
	std::unordered_map<std::string, std::shared_ptr<T>> Map;
	std::string Name;
	
	ResourceManager(const ResourceManager&) {};
	ResourceManager& operator = (const ResourceManager&) { return *this; };

	void ReleaseAll()
	{
		Map.clear();
	}

public:
	std::shared_ptr<T> Load(const std::string& filename, void* args = nullptr)
	{
		if (filename.empty())
			Log::Warn("Filename cannot be null");

		auto it = Map.find(filename);

		if (it != Map.end())
		{
			return (*it).second;
		}

		std::shared_ptr<T> resource = std::make_shared<T>(filename, args);

		Map.insert(std::make_pair(filename, resource));

		return resource;
	}

	std::shared_ptr<T> Load(std::shared_ptr<T> resource)
	{
		auto it = Map.find(resource->GetName());

		if (it != Map.end())
		{
			return (*it).second;
		}

		Map.insert(std::make_pair(resource->GetName(), resource));

		return resource;
	}

	bool Unload(const std::string& filename)
	{
		if (filename.empty())
			Log::Error("filename cannot be null");

		std::string FileName = filename;

		auto it = Map.find(FileName);

		if (it != Map.end())
		{
			Map.erase(it);
			return true;
		}

		Log::Error("cannot find " + FileName);

		return false;
	}

	void Initialise(const std::string& name) 
	{
		if (name.empty())
			Log::Error("Null name is not allowed");

		Name = name;
	}

	const std::string& GetName() const { return Name; }
	const int Size() const { return Map.size(); }

	ResourceManager() {}
	~ResourceManager() { ReleaseAll(); }
};

