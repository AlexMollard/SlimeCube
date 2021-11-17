#pragma once
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Entity.h"
#include "ImGuizmo.h"
#include "ContentBrowser.h"
#include "ResourceManager.h"
#include "MaterialComponentPanel.h"
class ImGuiLayer
{
public:
	static ImGuiLayer* GetInstance();

	ImGuiLayer(const ImGuiLayer&) = delete;
	ImGuiLayer(ImGuiLayer&&) = delete;
	ImGuiLayer& operator=(const ImGuiLayer&) = delete;
	ImGuiLayer& operator=(ImGuiLayer&&) = delete;

	static void Render(void* renderTex);
	static void SetScene(Scene* scene);
	static void DeleteInstance();

	static void StartFrame();
	static void EndFrame();
	static void SetGizmoState(ImGuizmo::OPERATION newState);
	static ImVec2 GetViewPortSize();

	void SetTextureManager(std::shared_ptr<ResourceManager<Texture>> manager);
private:
	ImGuiLayer();
	~ImGuiLayer();

	void OnAttach();

	void Hierarchy();
	void DrawEntityNode(Entity entity);

	void DrawMenuBar();
	void Properties(Entity entity);
	void DrawProperties(Entity entity);
	void FileExplorer();
	void DrawGizmos(Entity entity);
	void DrawViewPort(void* renderTex);

	void SetDarkThemeColors();

	ImGuiWindowFlags mainFlags =
	{
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoCollapse
	};

	static ImGuiLayer* instance;
	ImVec2 screenSize;
	float columnWidth, rowHeight;
	Scene* scene = nullptr;
	Entity selectionContext;
	glm::vec2 m_ViewportBounds[2];
	ImVec2 viewPortSize;
	ImGuizmo::OPERATION gizmoType = ImGuizmo::OPERATION::TRANSLATE;

	ContentBrowser contentBrowser;
	MaterialComponentPanel materialComponentPanel;
	std::shared_ptr<ResourceManager<Texture>> resourceManager;
};
