#include "pch.h"
#include "ContentBrowser.h"


static const std::filesystem::path assetsPath = "Assets";

ContentBrowser::ContentBrowser()
{
	currentDirectory = assetsPath.c_str();
	directoryIcon = Texture::Create("Assets/Images/Icons/Folder.png");
	fileIcon = Texture::Create("Assets/Images/Icons/File.png");
	pngIcon = Texture::Create("Assets/Images/Icons/PNG File.png");
	prevFrameDirectory = "False Directory";
}

void ContentBrowser::OnRender(ImVec2 panelPos, ImVec2 panelSize, ImGuiWindowFlags flags)
{
	ImGui::SetNextWindowPos(panelPos);
	ImGui::SetNextWindowSize(panelSize);

	ImGui::Begin("File Viewer", nullptr, flags);


	static float padding = 20.0f;
	static float thumbnailSize = 80.0f;
	float cellSize = thumbnailSize + padding;

	int coloumnCount = (int)(panelSize.x / cellSize);
	if (coloumnCount < 1)
		coloumnCount = 1;


	if (currentDirectory != assetsPath)
	{
		if (ImGui::Button("<-"))
		{
			currentDirectory = currentDirectory.parent_path();
			updateImages = true;
			imagesInDir.clear();
		}
	}

	ImGui::Columns(coloumnCount, nullptr, false);
	int id = 0;

	for (auto& dirEntry : std::filesystem::directory_iterator(currentDirectory))
	{
		const auto& path = dirEntry.path();
		auto relativePath = std::filesystem::relative(path, assetsPath);
		std::string fileNameString = relativePath.filename().string();

		ImGui::PushID(fileNameString.c_str());
		
		std::shared_ptr<Texture> icon = nullptr;
		if (path.extension() == ".png")
		{
			if (!updateImages)
			{
				if (imagesInDir.size() >= id + 1)
					icon = imagesInDir[id++];
				else
					icon = pngIcon;
			}
			else
			{
				imagesInDir.push_back(Texture::Create(path.string()));
				icon = imagesInDir.back();
				id++;
			}
		}
		else if (dirEntry.is_directory())
			icon = directoryIcon;
		else
			icon = fileIcon;

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		ImGui::ImageButton((ImTextureID)icon->GetID(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });


		if (ImGui::BeginDragDropSource())
		{
			const wchar_t* itemPath = relativePath.c_str();
			ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));
			ImGui::EndDragDropSource();
		}

		ImGui::PopStyleColor();
		if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
		{
			if (dirEntry.is_directory())
			{
				id = 0;
				updateImages = true;
				imagesInDir.clear();
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

	if (id == imagesInDir.size() && id != 0)
		updateImages = false;

	ImGui::Columns(1);

	ImGui::End();
}
