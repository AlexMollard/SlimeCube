#pragma once

#include "Scene.h"
#include "entt/entt.hpp"

class Entity
{
  public:
    Entity() = default;

    Entity(entt::entity handle, Scene* tempScene);

    Entity(const Entity& other) = default;

    template <typename T, typename... Args>
    T& AddComponent(Args&&... args)
    {
        if (HasComponent<T>()) {
            std::cout << "Object already has component" << std::endl;
            return scene->m_registry.get<T>(entityHandle);
        }

        T& component = scene->m_registry.emplace<T>(entityHandle, std::forward<Args>(args)...);
        return component;
    }

    template <typename T, typename... Args>
    T& AddOrReplaceComponent(Args&&... args)
    {
        T& component = scene->m_registry.emplace_or_replace<T>(entityHandle, std::forward<Args>(args)...);
        return component;
    }

    template <typename T>
    T& GetComponent()
    {
        if (!HasComponent<T>()) {
            std::cout << "Entity does not have component!" << std::endl;
            // return T{};
        }

        return scene->m_registry.get<T>(entityHandle);
    }

    template <typename T>
    bool HasComponent()
    {
        return scene->m_registry.all_of<T>(entityHandle);
    }

    template <typename T>
    void RemoveComponent()
    {
        scene->m_registry.remove<T>(entityHandle);
    }

    explicit operator bool() const { return entityHandle != entt::null; }

    operator entt::entity() const { return entityHandle; }

    explicit operator uint32_t() const { return (uint32_t)entityHandle; }

    bool operator==(const Entity& other) const { return entityHandle == other.entityHandle && scene == other.scene; }

    bool operator!=(const Entity& other) const { return !(*this == other); }

    Scene* scene = nullptr;
    entt::entity entityHandle{entt::null};

  private:
};
