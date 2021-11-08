#pragma once
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "SceneObject.h"


class EditorGUI
{
public:
	EditorGUI(Scene* scene);
	~EditorGUI();

	void OnAttach();

	void Render();
	void StartFrame();
	void EndFrame();

	void Hierarchy();
	void DrawEntityNode(SceneObject entity);

	void Properties(SceneObject entity);
	void DrawProperties(SceneObject entity);
	void Menu();
	void FileExplorer();
	void FileViewer();

	void SetDarkThemeColors();
private:
	ImGuiWindowFlags mainFlags =
	{
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoScrollbar |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoDecoration
	};

	ImVec2 screenSize;
	float columnWidth, rowHeight;
	Scene* scene = nullptr;
	SceneObject selectionContext;
};
