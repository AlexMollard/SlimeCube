#pragma once

#include "imgui.h"
#include <filesystem>
#include "Texture.h"
#include "ResourceHub.h"

class ContentBrowser {
public:
    ContentBrowser();

    ~ContentBrowser() = default;

    void AddTextures(std::shared_ptr<ResourceHub> resHub);

    void OnRender(std::shared_ptr<ResourceHub> resHub, ImVec2 panelPos, ImVec2 panelSize, ImGuiWindowFlags flags);

private:

    std::filesystem::path currentDirectory;
    std::filesystem::path prevFrameDirectory;

    std::shared_ptr<Texture> directoryIcon = nullptr;
    std::shared_ptr<Texture> fileIcon = nullptr;
    std::shared_ptr<Texture> pngIcon = nullptr;

    std::vector<std::shared_ptr<Texture>> imagesInDir;

    bool updateImages = true;
};

