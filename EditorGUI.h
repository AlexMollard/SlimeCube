#pragma once
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

class EditorGUI
{
public:
	EditorGUI();
	~EditorGUI();

	void Render();
	void StartFrame();
	void EndFrame();
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
};
