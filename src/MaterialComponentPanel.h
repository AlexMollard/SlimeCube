#pragma once

#include "Components.h"
#include "ResourceHub.h"
#include "imgui.h"

class MaterialComponentPanel
{
public:
 MaterialComponentPanel() = default;

 ~MaterialComponentPanel() = default;

 void OnRender(const std::shared_ptr<ResourceHub> &resHub, MaterialComponent &component, ImGuiWindowFlags flags);

private:
 static std::shared_ptr<Texture> DragAndDropTexture(const std::shared_ptr<ResourceHub> &resHub, MaterialComponent &component);

 ImVec2 imageSize = ImVec2(100, 100);
};
