#pragma once
#include "Scene.h"
#include "entt.hpp"
#include <iostream>

class SceneObject
{
public:
	SceneObject() = default;
	SceneObject(entt::entity handle, Scene* tempScene);
	SceneObject(const SceneObject& other) = default;

	template<typename T, typename... Args>
	T& AddComponant(Args&&... args)
	{
		if (HasComponent<T>()) 
		{
			std::cout << "Object already has componant" << std::endl;
			return scene->registry.get<T>(entityHandle);
		}

		T& componant = scene->registry.emplace<T>(entityHandle, std::forward<Args>(args)...);
		scene->OnComponentAdded<T>(*this, componant);
		return componant;
	}

	template<typename T, typename... Args>
	T& AddOrReplaceComponent(Args&&... args)
	{
		T& component = scene->registry.emplace_or_replace<T>(entityHandle, std::forward<Args>(args)...);
		scene->OnComponentAdded<T>(*this, component);
		return component;
	}

	template<typename T>
	T& GetComponent()
	{
		if (!HasComponent<T>()) 
		{
			std::cout << "Entity does not have component!" << std::endl;
			//return T{};
		}

		std::cout << scene->name << std::endl;
		return scene->registry.get<T>(entityHandle);
	}

	template<typename T>
	bool HasComponent()
	{
	//	std::cout << entityHandle << std::endl;
		return scene->registry.all_of<T>(entityHandle);
	}

	template<typename T>
	void RemoveComponent()
	{
		scene->registry.remove<T>(entityHandle);
	}

	operator bool() const { return entityHandle != entt::null; }
	operator entt::entity() const { return entityHandle; }
	operator uint32_t() const { return (uint32_t)entityHandle; }

	bool operator==(const SceneObject& other) const
	{
		return entityHandle == other.entityHandle && scene == other.scene;
	}

	bool operator!=(const SceneObject& other) const
	{
		return !(*this == other);
	}

	Scene* scene = nullptr;
	entt::entity entityHandle{ entt::null };
private:
};

