#include "SceneObject.h"

SceneObject::SceneObject(entt::entity handle, Scene* tempScene)
    : entityHandle(handle), scene(tempScene)
{

}