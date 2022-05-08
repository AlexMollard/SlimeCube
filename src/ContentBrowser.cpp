#include <iostream>
#include "ContentBrowser.h"
#include "Directories.h"

static const std::filesystem::path assetsPath = ASSETS_DIR;

ContentBrowser::ContentBrowser() {
    std::cout << std::filesystem::current_path() << std::endl;
    currentDirectory = assetsPath.c_str();
    directoryIcon = Texture::Create(Dir::FromAsset("/Images/Icons/Folder.png"));
    fileIcon = Texture::Create(Dir::FromAsset("/Images/Icons/File.png"));
    pngIcon = Texture::Create(Dir::FromAsset("/Images/Icons/PNG File.png"));
    prevFrameDirectory = "False Directory";
}

void ContentBrowser::AddTextures(std::shared_ptr<ResourceHub> resHub) {
    for (auto &dirEntry: std::filesystem::directory_iterator(currentDirectory)) {
        const std::string &path = dirEntry.path().string();

        if (!(dirEntry.path().extension() == ".png" || dirEntry.path().extension() == ".jpg"))
            continue;

        imagesInDir.push_back(resHub->GetTextureManager()->Load(path));
    }

}


void ContentBrowser::OnRender(std::shared_ptr<ResourceHub> resHub, ImVec2 panelPos, ImVec2 panelSize,
                              ImGuiWindowFlags flags) {
    ImGui::SetNextWindowPos(panelPos);
    ImGui::SetNextWindowSize(panelSize);

    ImGui::Begin("File Viewer", nullptr, flags);


    static float padding = 20.0f;
    static float thumbnailSize = 80.0f;
    float cellSize = thumbnailSize + padding;

    int coloumnCount = (int) (panelSize.x / cellSize);
    if (coloumnCount < 1)
        coloumnCount = 1;


    if (currentDirectory != assetsPath) {
        if (ImGui::Button("<-")) {
            currentDirectory = currentDirectory.parent_path();
            updateImages = true;
        }
    }

    ImGui::Columns(coloumnCount, nullptr, false);
    int id = 0;

    if (updateImages) {
// True (1) then will remove textures if not used in scene to free memory
// this will mean you will have to reload each texture each time you load them
#if 0
        for (auto i : imagesInDir)
        {
            if (i.use_count() <= 3)
            {
                resHub->GetTextureManager()->Unload(i->GetName());
            }
        }
#endif

        imagesInDir.clear();
        AddTextures(resHub);
        updateImages = false;
    }

    for (auto &dirEntry: std::filesystem::directory_iterator(currentDirectory)) {
        const auto &path = dirEntry.path();
        auto relativePath = std::filesystem::relative(path, assetsPath);
        std::string fileNameString = relativePath.filename().string();

        ImGui::PushID(fileNameString.c_str());

        std::shared_ptr<Texture> icon = nullptr;
        if (path.extension() == ".png" || path.extension() == ".jpg") {
            if (!updateImages) {
                if (imagesInDir.size() >= id + 1)
                    icon = imagesInDir[id++];
                else
                    icon = pngIcon;
            } else {
                icon = pngIcon;
            }
        } else if (dirEntry.is_directory())
            icon = directoryIcon;
        else
            icon = fileIcon;

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
        ImGui::ImageButton((ImTextureID) icon->GetID(), {thumbnailSize, thumbnailSize}, {0, 1}, {1, 0});


        if (ImGui::BeginDragDropSource()) {
            const wchar_t *itemPath = relativePath.c_str();
            ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));
            ImGui::ImageButton((ImTextureID) icon->GetID(), {thumbnailSize * 0.75f, thumbnailSize * 0.75f}, {0, 1},
                               {1, 0});
            ImGui::Text(fileNameString.c_str());
            ImGui::EndDragDropSource();
        }

        ImGui::PopStyleColor();
        if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
            if (dirEntry.is_directory()) {
                id = 0;
                updateImages = true;
                currentDirectory /= path.filename();

                ImGui::TextWrapped(fileNameString.c_str());
                ImGui::NextColumn();
                ImGui::PopID();

                break;
            }
        }

        ImGui::TextWrapped(fileNameString.c_str());
        ImGui::NextColumn();
        ImGui::PopID();
    }

    ImGui::Columns(1);

    ImGui::End();
}
