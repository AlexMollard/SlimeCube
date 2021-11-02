#include "EditorGUI.h"
#include "Input.h"
#include <string>

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
	screenSize.x = Input::GetInstance()->GetWindowSize().x;
	screenSize.y = Input::GetInstance()->GetWindowSize().y;
	columnWidth = screenSize.x * 0.15f;
	rowHeight = screenSize.x * 0.20f;
	
	StartFrame();

	Hierarchy();
	Properties();
	Menu();
	FileExplorer();
	FileViewer();

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

void EditorGUI::Hierarchy()
{
	ImGui::SetNextWindowPos(ImVec2(screenSize.x * 0.85f, 0.0f));
	ImGui::SetNextWindowSize(ImVec2(columnWidth, screenSize.y * 0.50f));
	ImGui::Begin("Hierarchy", nullptr, mainFlags);
	ImGui::End();
}

void EditorGUI::Properties()
{
	std::string buf = "Hello";
	buf.resize(32);

	glm::vec3 pos = glm::vec3(12.0f, 1.0f, 89.0f);
	float newPos[3] = { pos.x,pos.y,pos.z };
	bool isActive = true;


	ImGui::SetNextWindowPos(ImVec2(screenSize.x * 0.85f, screenSize.y * 0.50f));
	ImGui::SetNextWindowSize(ImVec2(columnWidth, screenSize.y * 0.50f + 2.0f));
	ImGui::Begin("Properties", nullptr, mainFlags);

	ImGui::Text("Active:");
	ImGui::Checkbox("", &isActive);

	ImGui::Text("Name:");
	ImGui::InputText("", (char*)buf.c_str(), 32);

	ImGui::Text("Position:");
	ImGui::InputFloat3("", newPos);

	ImGui::Text("Rotation:");
	ImGui::InputFloat3("", newPos);

	ImGui::Text("Scale:");
	ImGui::InputFloat3("", newPos);

	ImGui::End();
}

void EditorGUI::Menu()
{
	ImGui::SetNextWindowPos(ImVec2(screenSize.x * 0.85f - 95.0f, screenSize.y * 0.80f - 50.0f));
	ImGui::SetNextWindowSize(ImVec2(95.0f, 50.0f));
	ImGui::Begin("Play and Pause", nullptr, mainFlags);
	ImGui::ColorButton("Play", ImVec4(0.3f, 0.9f, 0.3f, 1.0f), ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoDragDrop, ImVec2(35.0f, 35.0f));
	ImGui::SameLine();
	ImGui::ColorButton("Stop", ImVec4(0.9f, 0.3f, 0.3f, 1.0f), ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoDragDrop, ImVec2(35.0f, 35.0f));
	ImGui::End();
}

void EditorGUI::FileExplorer()
{
	ImGui::SetNextWindowPos(ImVec2(0.0f, screenSize.y * 0.80f));
	ImGui::SetNextWindowSize(ImVec2(screenSize.x * 0.70f, rowHeight + 1.0f));
	ImGui::Begin("File Explorer", nullptr, mainFlags);
	ImGui::End();
}

void EditorGUI::FileViewer()
{
	ImGui::SetNextWindowPos(ImVec2(screenSize.x * 0.70f, screenSize.y * 0.80f));
	ImGui::SetNextWindowSize(ImVec2(screenSize.x * 0.15f, rowHeight + 1.0f));
	ImGui::Begin("File Viewer", nullptr, mainFlags);
	ImGui::End();
}
