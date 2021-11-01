#include "EditorGUI.h"
#include "Input.h"

EditorGUI::EditorGUI()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(Input::GetInstance()->GetWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 130");
}

EditorGUI::~EditorGUI()
{
	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void EditorGUI::Render()
{
	StartFrame();
	screenSize.x = Input::GetInstance()->GetWindowSize().x;
	screenSize.y = Input::GetInstance()->GetWindowSize().y;
	ImGui::BeginMainMenuBar();

	if (ImGui::BeginMenu("File"))
	{
		if (ImGui::MenuItem("New Scene", "CTRL+N")) {}
		if (ImGui::MenuItem("Open Scene", "CTRL+O")) {}
		if (ImGui::MenuItem("Save Scene", "CTRL+S")) {}

		ImGui::EndMenu();
	}
	ImGui::EndMainMenuBar();

	ImGui::SetNextWindowPos(ImVec2(-0.5f, 18.0f));
	ImGui::SetNextWindowSize(ImVec2(screenSize.x * 0.20f, screenSize.y * 0.80f - 18.0f));
	ImGui::Begin("Hierarchy", nullptr, mainFlags);
	ImGui::End();

	ImGui::SetNextWindowPos(ImVec2(screenSize.x * 0.80f + 1.0f, 18.0f));
	ImGui::SetNextWindowSize(ImVec2(screenSize.x * 0.20f, screenSize.y * 0.80f - 18.0f));
	ImGui::Begin("Properties", nullptr, mainFlags);
	ImGui::End();

	ImGui::SetNextWindowPos(ImVec2(screenSize.x * 0.20f, 18.0f));
	ImGui::SetNextWindowSize(ImVec2(screenSize.x * 0.601f, 50.0f));
	ImGui::Begin("Play and Pause", nullptr, mainFlags);
	ImGui::ColorButton("Play", ImVec4(0.3f, 0.9f, 0.3f, 1.0f), ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoDragDrop, ImVec2(35.0f, 35.0f));
	ImGui::SameLine();
	ImGui::ColorButton("Stop", ImVec4(0.9f, 0.3f, 0.3f, 1.0f), ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoDragDrop, ImVec2(35.0f, 35.0f));
	ImGui::End();

	ImGui::SetNextWindowPos(ImVec2(0.0f, screenSize.y * 0.80f));
	ImGui::SetNextWindowSize(ImVec2(screenSize.x, screenSize.y * 0.20f + 1.0f));
	ImGui::Begin("File Explorer", nullptr, mainFlags);
	ImGui::End();

	EndFrame();
}

void EditorGUI::StartFrame()
{
	// ImGui Stuff Below
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void EditorGUI::EndFrame()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}