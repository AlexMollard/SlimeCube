#pragma once
#include "imgui.h"
#include "ResourceHub.h"
#include "Components.h"

class PointLightComponentPanel
{
public:
	PointLightComponentPanel() = default;
	~PointLightComponentPanel() = default;
	
	void OnRender(std::shared_ptr<ResourceHub> resHub, PointLightComponent& component, ImGuiWindowFlags flags);
private:
	void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f);
};

