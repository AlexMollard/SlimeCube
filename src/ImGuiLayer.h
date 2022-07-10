#pragma once

#include <ImGuizmo.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "ContentBrowser.h"
#include "Entity.h"
#include "MaterialComponentPanel.h"
#include "PointLightComponentPanel.h"
#include "ResourceManager.h"

class ImGuiLayer
{
public:
 static ImGuiLayer *GetInstance();

 ImGuiLayer(const ImGuiLayer &) = delete;

 ImGuiLayer(ImGuiLayer &&) = delete;

 ImGuiLayer &operator=(const ImGuiLayer &) = delete;

 ImGuiLayer &operator=(ImGuiLayer &&) = delete;

 static void Render(void *renderTex);

 static void SetScene(Scene *scene);

 static void DeleteInstance();

 static void StartFrame();

 static void EndFrame();

 static void SetGizmoState(ImGuizmo::OPERATION newState);

 static ImVec2 GetViewPortSize();

private:
 ImGuiLayer();

 ~ImGuiLayer();

 static void OnAttach();

 void Hierarchy();

 void DrawEntityNode(Entity entity);

 static void DrawMenuBar();

 void Properties(Entity entity) const;

 static void DrawProperties(Entity entity);

 void FileExplorer();

 void DrawGizmos(Entity entity);

 void DrawViewPort(void *renderTex);

 static void SetDarkThemeColors();

 ImGuiWindowFlags mainFlags = {ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse};

 static ImGuiLayer *instance;
 ImVec2 screenSize = {0, 0};
 float columnWidth = 0.0f;
 float rowHeight = 0.0f;
 Scene *scene = nullptr;
 Entity selectionContext = {};
 glm::vec2 m_ViewportBounds[2] = {};
 ImVec2 viewPortSize = {0, 0};
 ImGuizmo::OPERATION gizmoType = ImGuizmo::OPERATION::TRANSLATE;

 ContentBrowser contentBrowser = ContentBrowser();
 MaterialComponentPanel materialComponentPanel = MaterialComponentPanel();
 PointLightComponentPanel pointLightComponentPanel = PointLightComponentPanel();

 std::shared_ptr<ResourceManager<Texture>> resourceManager;
};
