#include "MaterialComponentPanel.h"

void MaterialComponentPanel::OnRender(
    const std::shared_ptr<ResourceHub> &resHub, MaterialComponent &component, ImGuiWindowFlags flags)
{
 Material *mat = component.material.get();
 static ImVec2 size;
 ImGui::SetNextWindowSize(size);

 ImGuiIO &io = ImGui::GetIO();
 auto boldFont = io.Fonts->Fonts[0];

 // Material Name
 ImGui::Text("Name: ");
 ImGui::SameLine();
 auto &tag = mat->GetName();
 char buffer[256];
 memset(buffer, 0, sizeof(buffer));
 strncpy_s(buffer, tag.c_str(), sizeof(buffer));
 if (ImGui::InputText("##Tag", buffer, sizeof(buffer))) {
  mat->SetName(std::string(buffer));
 }

 ImGui::PushFont(boldFont);

 ImGui::Columns(2, nullptr, false);

 // Albedo
 ImTextureID id = (mat->GetAlbedo() != nullptr) ? (ImTextureID)mat->GetAlbedo()->GetID() : nullptr;

 ImGui::TextWrapped("Albedo");
 ImGui::ImageButton(id, imageSize, {0, 1}, {1, 0});
 if (auto tex = DragAndDropTexture(resHub, component)) component.material->SetAlbedo(tex);
 ImGui::NextColumn();

 // Specular Map
 id = (mat->GetSpecMap() != nullptr) ? (ImTextureID)mat->GetSpecMap()->GetID() : nullptr;

 ImGui::TextWrapped("Specular");
 ImGui::ImageButton(id, imageSize, {0, 1}, {1, 0});
 if (auto tex = DragAndDropTexture(resHub, component)) component.material->SetSpecMap(tex);
 ImGui::NextColumn();

 // Normal Map
 id = (mat->GetNormalMap() != nullptr) ? (ImTextureID)mat->GetNormalMap()->GetID() : nullptr;

 ImGui::TextWrapped("Normal");
 ImGui::ImageButton(id, imageSize, {0, 1}, {1, 0});
 if (auto tex = DragAndDropTexture(resHub, component)) component.material->SetNormalMap(tex);
 ImGui::NextColumn();

 // Ambient Map
 id = (mat->GetAmbientMap() != nullptr) ? (ImTextureID)mat->GetAmbientMap()->GetID() : nullptr;

 ImGui::TextWrapped("Ambient");
 ImGui::ImageButton(id, imageSize, {0, 1}, {1, 0});
 if (auto tex = DragAndDropTexture(resHub, component)) component.material->SetAmbientMap(tex);
 ImGui::NextColumn();

 // Rough Map
 id = (mat->GetRoughMap() != nullptr) ? (ImTextureID)mat->GetRoughMap()->GetID() : nullptr;

 ImGui::TextWrapped("Rough");
 ImGui::ImageButton(id, imageSize, {0, 1}, {1, 0});
 if (auto tex = DragAndDropTexture(resHub, component)) component.material->SetRoughMap(tex);
 ImGui::NextColumn();

 // Displacement Map
 id = (mat->GetDisplacementMap() != nullptr) ? (ImTextureID)mat->GetDisplacementMap()->GetID() : nullptr;

 ImGui::TextWrapped("Displacement");
 ImGui::ImageButton(id, imageSize, {0, 1}, {1, 0});
 if (auto tex = DragAndDropTexture(resHub, component)) component.material->SetDisplacementMap(tex);

 ImGui::Columns(1);
 ImGui::PopFont();
}

std::shared_ptr<Texture> MaterialComponentPanel::DragAndDropTexture(
    const std::shared_ptr<ResourceHub> &resHub, MaterialComponent &component)
{
 if (ImGui::BeginDragDropTarget()) {
  if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")) {
   // Your wchar_t*
   std::wstring ws((const wchar_t *)payload->Data);
   // your new String
   std::string str(ws.begin(), ws.end());
   ImGui::EndDragDropTarget();
   return resHub->GetTextureManager()->Load("../../Assets/" + str);
  }

  ImGui::EndDragDropTarget();
 }

 return nullptr;
}
