#include "SceneObject.h"

SceneObject::SceneObject(entt::entity handle, Scene* scene)
    : entityHandle(handle), scene(scene)
{

}