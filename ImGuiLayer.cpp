#include "pch.h"
#include "ImGuiLayer.h"
#include "imgui_internal.h"
#include "Input.h"
#include "Components.h"
#include "glm.hpp"
#include "Math.h"

ImGuiLayer* ImGuiLayer::GetInstance()
{
	if (!instance)
	{
		instance = new ImGuiLayer();
	}
	
	return instance;
}

ImGuiLayer::ImGuiLayer()
{
	Log::Info(__func__ + std::string(" Instance Created"));
	OnAttach();
}

ImGuiLayer::~ImGuiLayer()
{
	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

}

void ImGuiLayer::SetScene(Scene* scene)
{
	GetInstance()->scene = scene;
}

void ImGuiLayer::DeleteInstance()
{	
	delete GetInstance();
}

void ImGuiLayer::OnAttach()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();
	float fontSize = 18.0f;
	io.Fonts->AddFontFromFileTTF("Assets/Fonts/opensans/OpenSans-Bold.ttf", fontSize);
	io.FontDefault = io.Fonts->AddFontFromFileTTF("Assets/Fonts/opensans/OpenSans-Regular.ttf",fontSize);
	ImGui_ImplGlfw_InitForOpenGL(Input::GetInstance()->GetWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 130");

	ImGui::StyleColorsDark();
	SetDarkThemeColors();

	ImGuiStyle& style = ImGui::GetStyle();
}

void ImGuiLayer::Render(void* renderTex)
{
	GetInstance()->screenSize.x = Input::GetInstance()->GetWindowSize().x;
	GetInstance()->screenSize.y = Input::GetInstance()->GetWindowSize().y;
	GetInstance()->columnWidth = 380.0f;
	GetInstance()->rowHeight = 230.0f;

	GetInstance()->Hierarchy();
	GetInstance()->Properties(GetInstance()->selectionContext);
	GetInstance()->FileExplorer();
	GetInstance()->DrawViewPort(renderTex);
	GetInstance()->DrawGizmos(GetInstance()->selectionContext);
	GetInstance()->DrawMenuBar();
}

void ImGuiLayer::StartFrame()
{
	// ImGui Stuff Below
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGuizmo::BeginFrame();
}

void ImGuiLayer::EndFrame()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiLayer::SetGizmoState(ImGuizmo::OPERATION newState)
{
	GetInstance()->gizmoType = newState;
}

ImVec2 ImGuiLayer::GetViewPortSize()
{
	return GetInstance()->viewPortSize;
}

void ImGuiLayer::Hierarchy()
{
	ImGui::SetNextWindowPos(ImVec2(screenSize.x - columnWidth, 23.0f));
	ImGui::SetNextWindowSize(ImVec2(columnWidth, screenSize.y * 0.50f - 23.0f));
	ImGui::Begin("Scene Hierarchy", nullptr, mainFlags);

	scene->registry.each([&](auto entityID)
		{
			Entity entity{ entityID , scene };
			DrawEntityNode(entity);
		});

	if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
		selectionContext = {};
	
	if (ImGui::BeginPopupContextWindow(0,1,false))
	{
		if (ImGui::MenuItem("Create Empty Object"))
			selectionContext = scene->CreateEntity("Empty Object");

		ImGui::EndPopup();
	}

	ImGui::End();
}

void ImGuiLayer::DrawEntityNode(Entity entity)
{
	auto& tag = entity.GetComponent<TagComponent>().Tag;

	ImGuiTreeNodeFlags flags = ((selectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
	flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
	bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
	if (ImGui::IsItemClicked())
	{
		selectionContext = entity;
	}

	bool entityDeleted = false;
	if (ImGui::BeginPopupContextItem())
	{
		if (ImGui::MenuItem("Delete Entity"))
			entityDeleted = true;

		ImGui::EndPopup();
	}

	if (opened)
	{
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
		bool opened = ImGui::TreeNodeEx((void*)9817239, flags, tag.c_str());
		if (opened)
			ImGui::TreePop();
		ImGui::TreePop();
	}
	
	if (entityDeleted)
	{
		scene->DestroyEntity(entity);
		if (selectionContext == entity)
			selectionContext = {};
	}
}

void ImGuiLayer::DrawMenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Project"))
		{
			if (ImGui::MenuItem("New", "Ctrl+N")) {}

			if (ImGui::MenuItem("Open", "Ctrl+O")) {}

			if (ImGui::MenuItem("Save", "Ctrl+S")) {}

			if (ImGui::MenuItem("Save As", "Ctrl+Shift+S")) {}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Scene"))
		{
			if (ImGui::MenuItem("New")) {}
			
			if (ImGui::MenuItem("Open")) {}
			
			if (ImGui::MenuItem("Save")) {}

			if (ImGui::MenuItem("Save As")) {}
		
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Create Entity"))
		{
			if (ImGui::MenuItem("Cube")) {}

			if (ImGui::MenuItem("Quad")) {}

			if (ImGui::MenuItem("Plane")) {}

			if (ImGui::MenuItem("Torus")) {}

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
}

void ImGuiLayer::Properties(Entity entity)
{
	ImGui::SetNextWindowPos(ImVec2(screenSize.x - columnWidth, screenSize.y * 0.50f));
	ImGui::SetNextWindowSize(ImVec2(columnWidth, screenSize.y * 0.50f + 2.0f));

	ImGui::Begin("Properties", nullptr, mainFlags);

	if (entity)
		DrawProperties(entity);
	// Second one is to just fill up properties panel for testing
	if (entity)
		DrawProperties(entity);
	
	ImGui::End();
}

static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f)
{
	ImGuiIO& io = ImGui::GetIO();
	auto boldFont = io.Fonts->Fonts[0];

	ImGui::PushID(label.c_str());

	ImGui::Columns(2);
	ImGui::SetColumnWidth(0, columnWidth);
	ImGui::Text(label.c_str());
	ImGui::NextColumn();

	ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

	float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
	ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
	ImGui::PushFont(boldFont);
	if (ImGui::Button("X", buttonSize))
		values.x = resetValue;
	ImGui::PopFont();
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();
	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
	ImGui::PushFont(boldFont);
	if (ImGui::Button("Y", buttonSize))
		values.y = resetValue;
	ImGui::PopFont();
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();
	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
	ImGui::PushFont(boldFont);
	if (ImGui::Button("Z", buttonSize))
		values.z = resetValue;
	ImGui::PopFont();
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();

	ImGui::PopStyleVar();

	ImGui::Columns(1);

	ImGui::PopID();
}

template<typename T, typename UIFunction>
static void DrawComponent(const std::string& name, Entity entity, UIFunction uiFunction)
{
	const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
	if (entity.HasComponent<T>())
	{
		auto& component = entity.GetComponent<T>();
		ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImGui::Separator();
		bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());
		ImGui::PopStyleVar(
		);
		ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
		if (ImGui::Button("+", ImVec2{ lineHeight, lineHeight }))
		{
			ImGui::OpenPopup("ComponentSettings");
		}

		bool removeComponent = false;
		if (ImGui::BeginPopup("ComponentSettings"))
		{
			if (ImGui::MenuItem("Remove component"))
				removeComponent = true;

			ImGui::EndPopup();
		}

		if (open)
		{
			uiFunction(component);
			ImGui::TreePop();
		}

		if (removeComponent)
			entity.RemoveComponent<T>();
	}
}

void ImGuiLayer::DrawProperties(Entity entity)
{
	if (entity.HasComponent<TagComponent>())
	{
		auto& tag = entity.GetComponent<TagComponent>().Tag;

		char buffer[256];
		memset(buffer, 0, sizeof(buffer));
		strncpy_s(buffer, tag.c_str(), sizeof(buffer));
		if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
		{
			tag = std::string(buffer);
		}
	}

	ImGui::SameLine();

	if (ImGui::Button("Add Component"))
			ImGui::OpenPopup("AddComponent");

	if (ImGui::BeginPopup("AddComponent"))
	{ 
		if (!entity.HasComponent<TransformComponent>())
		{
			if (ImGui::MenuItem("Transform"))
			{
				entity.AddComponent<TransformComponent>();
				ImGui::CloseCurrentPopup();
			}
		}

		if (!entity.HasComponent<ShaderComponent>())
		{
			if (ImGui::MenuItem("Shader"))
			{
				entity.AddComponent<ShaderComponent>();
				ImGui::CloseCurrentPopup();
			}
		}

		if (!entity.HasComponent<MeshComponent>())
		{
			if (ImGui::MenuItem("Mesh"))
			{
				entity.AddComponent<MeshComponent>();
				ImGui::CloseCurrentPopup();
			}
		}

		if (!entity.HasComponent<MaterialComponent>())
		{
			if (ImGui::MenuItem("Material"))
			{
				entity.AddComponent<MaterialComponent>();
				ImGui::CloseCurrentPopup();
			}
		}

		ImGui::EndPopup();
	}

	DrawComponent<TransformComponent>("Transform", entity, [](auto& component)
	{
			DrawVec3Control("Translation", component.Translation);
			glm::vec3 rotation = glm::degrees(component.Rotation);
			DrawVec3Control("Rotation", rotation);
			component.Rotation = glm::radians(rotation);
			DrawVec3Control("Scale", component.Scale, 1.0f);

			if (ImGui::Button("Reset Values"))
			{
				component.Translation = glm::vec3(0.0f);
				component.Rotation = glm::vec3(0.0f);
				component.Scale = glm::vec3(1.0f);
			}
	});

	if (entity.HasComponent<ShaderComponent>())
	{
		DrawComponent<ShaderComponent>("Shader", entity, [](auto& component) 
		{


				ImGui::LabelText("Shader Crap", "");
		});
	}

	if (entity.HasComponent<MeshComponent>())
	{
		DrawComponent<MeshComponent>("Mesh", entity, [](auto& component) 
			{
				ImGui::LabelText("Mesh Crap", "");
			});
	}

	if (entity.HasComponent<MaterialComponent>())
	{
		DrawComponent<MaterialComponent>("Material", entity, [](auto& component) 
			{
				ImGui::LabelText("Material Crap", "");
				if (ImGui::ImageButton((ImTextureID)component.material->GetAlbedo()->GetID(), { 300,300 }))
				{
				}

				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
					{
						// Your wchar_t*
						std::wstring ws((const wchar_t*)payload->Data);
						// your new String
						std::string str(ws.begin(), ws.end());
						Texture* tempTex = new Texture("temp", "Assets\\" + str);	//MEMORY LEAK Needs to be std::shared_pointer (But need a texture manager or some sort of storing system)
						component.material->SetAlbedo(tempTex);
					}

					ImGui::EndDragDropTarget();
				}
			
			});
	}

	if (entity.HasComponent<SkyBoxComponent>())
	{
		DrawComponent<SkyBoxComponent>("SkyBox", entity, [](auto& component)
			{
				DrawVec3Control("Position", component.cam->Position);
				component.cam->UpdateProjectionViewMatrix();
			});
	}
}

void ImGuiLayer::FileExplorer()
{
	contentBrowser.OnRender(ImVec2(0.0f, screenSize.y - rowHeight), ImVec2((screenSize.x - (columnWidth)), rowHeight + 1.0f),mainFlags);
}

void ImGuiLayer::DrawGizmos(Entity entity)
{
	if (!entity)
	{
		ImGui::End();
		ImGui::PopStyleVar();
		return;
	}
	ImGuizmo::SetOrthographic(false);
	ImGuizmo::SetDrawlist();
	ImGuizmo::SetRect(m_ViewportBounds[0].x, m_ViewportBounds[0].y, m_ViewportBounds[1].x - m_ViewportBounds[0].x, m_ViewportBounds[1].y - m_ViewportBounds[0].y);

	// Editor camera
	const glm::mat4& cameraProjection = scene->GetMainCamera()->GetProjectionMatrix();
	glm::mat4 cameraView = scene->GetMainCamera()->GetViewMatrix();

	// Entity transform
	auto& tc = entity.GetComponent<TransformComponent>();
	glm::mat4 transform = tc.GetTransform();

	// Snapping
	bool snap = Input::GetKeyPress(LEFT_CONTROL);
	float snapValue = 0.5f; // Snap to 0.5m for translation/scale
	// Snap to 45 degrees for rotation
	if (gizmoType == ImGuizmo::OPERATION::ROTATE)
		snapValue = 45.0f;

	float snapValues[3] = { snapValue, snapValue, snapValue };

	ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
		(ImGuizmo::OPERATION)gizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform),
		nullptr, snap ? snapValues : nullptr);

	if (ImGuizmo::IsUsing())
	{
		glm::vec3 translation, rotation, scale;
		SlimeMath::DecomposeTransform(transform, translation, rotation, scale);

		glm::vec3 deltaRotation = rotation - tc.Rotation;
		tc.Translation = translation;
		tc.Rotation += deltaRotation;
		tc.Scale = scale;
	}

	ImGui::End();
	ImGui::PopStyleVar();
}

void ImGuiLayer::DrawViewPort(void* renderTex)
{
	ImGui::SetNextWindowPos(ImVec2(0.0f,23.0f));
	ImVec2 windowSize = ImVec2(screenSize.x - columnWidth, screenSize.y - rowHeight - 23.0f);
	ImGui::SetNextWindowSize(windowSize);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
	ImGui::Begin("Viewport", nullptr, mainFlags | ImGuiWindowFlags_NoDecoration);
	viewPortSize = ImGui::GetContentRegionAvail();
	auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
	auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
	auto viewportOffset = ImGui::GetWindowPos();
	m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
	m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

	bool viewportFocused = ImGui::IsWindowFocused();
	bool viewportHovered = ImGui::IsWindowHovered();

	ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
	windowSize = { viewportPanelSize.x, viewportPanelSize.y };
	
	ImGui::Image(reinterpret_cast<void*>(renderTex), windowSize, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

}

void ImGuiLayer::SetDarkThemeColors()
{
	auto& colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };

	// Headers
	colors[ImGuiCol_Header] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
	colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
	colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

	// Buttons
	colors[ImGuiCol_Button] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
	colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
	colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

	// Frame BG
	colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
	colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
	colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

	// Tabs
	colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.3805f, 0.381f, 1.0f };
	colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };
	colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };

	// Title
	colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
}