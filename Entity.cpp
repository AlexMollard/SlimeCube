#include "pch.h"
#include "Entity.h"
Entity::Entity(entt::entity handle, Scene* tempScene)
    : entityHandle(handle), scene(tempScene)
{

}