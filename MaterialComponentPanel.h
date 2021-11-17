#pragma once
#include "imgui.h"
#include "ResourceHub.h"
#include "Components.h"

class MaterialComponentPanel
{
public:
	MaterialComponentPanel();
	~MaterialComponentPanel() = default;
	void OnRender(std::shared_ptr<ResourceHub> resHub, MaterialComponent& component, ImGuiWindowFlags flags);
private:
	std::shared_ptr<Texture> DragAndDropTexture(std::shared_ptr<ResourceHub> resHub, MaterialComponent& component);

	ImVec2 imageSize = ImVec2(100,100);
};

