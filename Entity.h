#pragma once
#include "Scene.h"
#include "entt.hpp"
#include <iostream>

class Entity
{
public:
	Entity() = default;
	Entity(entt::entity handle, Scene* tempScene);
	Entity(const Entity& other) = default;

	template<typename T, typename... Args>
	T& AddComponent(Args&&... args)
	{
		if (HasComponent<T>()) 
		{
			std::cout << "Object already has componant" << std::endl;
			return scene->registry.get<T>(entityHandle);
		}

		T& componant = scene->registry.emplace<T>(entityHandle, std::forward<Args>(args)...);
		return componant;
	}

	template<typename T, typename... Args>
	T& AddOrReplaceComponent(Args&&... args)
	{
		T& component = scene->registry.emplace_or_replace<T>(entityHandle, std::forward<Args>(args)...);
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

	bool operator==(const Entity& other) const
	{
		return entityHandle == other.entityHandle && scene == other.scene;
	}

	bool operator!=(const Entity& other) const
	{
		return !(*this == other);
	}

	Scene* scene = nullptr;
	entt::entity entityHandle{ entt::null };
private:
};

