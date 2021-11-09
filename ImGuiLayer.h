#pragma once
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Entity.h"


class ImGuiLayer
{
public:
	static ImGuiLayer* GetInstance();

	ImGuiLayer(const ImGuiLayer&) = delete;
	ImGuiLayer(ImGuiLayer&&) = delete;
	ImGuiLayer& operator=(const ImGuiLayer&) = delete;
	ImGuiLayer& operator=(ImGuiLayer&&) = delete;

	static void Render();
	static void SetScene(Scene* scene);
	static void DeleteInstance();
private:
	ImGuiLayer();
	~ImGuiLayer();

	void OnAttach();
	void StartFrame();
	void EndFrame();

	void Hierarchy();
	void DrawEntityNode(Entity entity);

	void Properties(Entity entity);
	void DrawProperties(Entity entity);
	void Menu();
	void FileExplorer();
	void FileViewer();
	void DrawGizmos(Entity entity);

	void SetDarkThemeColors();

	ImGuiWindowFlags mainFlags =
	{
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoScrollbar |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoDecoration
	};

	static ImGuiLayer* instance;
	ImVec2 screenSize;
	float columnWidth, rowHeight;
	Scene* scene = nullptr;
	Entity selectionContext;
};
