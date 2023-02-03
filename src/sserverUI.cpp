#pragma once
#include "serverUI.h"
 
void myUI::Update() {
  SetMenuTheme();
  ImGui::Begin("ServerUI", 0,
               ImGuiWindowFlags_NoScrollbar |
                   ImGuiWindowFlags_NoScrollWithMouse |
                   ImGuiWindowFlags_NoResize);

  RenderMenu();  // rendering main window

  ImGui::End();
  ImGui::EndFrame();
}

void myUI::SetMenuTheme() {
  ImGuiStyle* style = &ImGui::GetStyle();

  style->WindowBorderSize = 0;
  style->WindowTitleAlign = ImVec2(0.5, 0.5);
  style->WindowMinSize = ImVec2(900, 430);

  style->FramePadding = ImVec2(8, 6);

  style->Colors[ImGuiCol_TitleBg] = ImColor(255, 101, 53, 255);
  style->Colors[ImGuiCol_TitleBgActive] = ImColor(255, 101, 53, 255);
  style->Colors[ImGuiCol_TitleBgCollapsed] = ImColor(0, 0, 0, 130);

  style->Colors[ImGuiCol_Button] = ImColor(31, 30, 31, 255);
  style->Colors[ImGuiCol_ButtonActive] = ImColor(31, 30, 31, 255);
  style->Colors[ImGuiCol_ButtonHovered] = ImColor(41, 40, 41, 255);

  style->Colors[ImGuiCol_Separator] = ImColor(70, 70, 70, 255);
  style->Colors[ImGuiCol_SeparatorActive] = ImColor(76, 76, 76, 255);
  style->Colors[ImGuiCol_SeparatorHovered] = ImColor(76, 76, 76, 255);

  style->Colors[ImGuiCol_FrameBg] = ImColor(37, 36, 37, 255);
  style->Colors[ImGuiCol_FrameBgActive] = ImColor(37, 36, 37, 255);
  style->Colors[ImGuiCol_FrameBgHovered] = ImColor(37, 36, 37, 255);

  style->Colors[ImGuiCol_Header] = ImColor(0, 0, 0, 0);
  style->Colors[ImGuiCol_HeaderActive] = ImColor(0, 0, 0, 0);
  style->Colors[ImGuiCol_HeaderHovered] = ImColor(46, 46, 46, 255);
}

void myUI::RenderMenu() {
  ImGui::Columns(2);
  ImGui::SetColumnOffset(1, 230);

  {
    // Left side

    static ImVec4 active = imguipp::to_vec4(41, 40, 41, 255);
    static ImVec4 inactive = imguipp::to_vec4(31, 30, 31, 255);

    ImGui::PushStyleColor(ImGuiCol_Button,
                          settings::Tab == 1 ? active : inactive);
    if (ImGui::Button(ICON_FA_HOURGLASS_START " Run Server",
                      ImVec2(230 - 15, 41)))
      settings::Tab = 1;

    ImGui::Spacing();
    ImGui::PushStyleColor(ImGuiCol_Button,
                          settings::Tab == 2 ? active : inactive);
    if (ImGui::Button(ICON_FA_USERS " Users", ImVec2(230 - 15, 41)))
      settings::Tab = 2;

    ImGui::Spacing();
    ImGui::PushStyleColor(ImGuiCol_Button,
                          settings::Tab == 3 ? active : inactive);
    if (ImGui::Button(ICON_FA_CODE " Logs", ImVec2(230 - 15, 41)))
      settings::Tab = 3;

    ImGui::Spacing();
    ImGui::PushStyleColor(ImGuiCol_Button,
                          settings::Tab == 4 ? active : inactive);
    if (ImGui::Button(ICON_FA_BOOK " Menu", ImVec2(230 - 15, 41)))
      settings::Tab = 4;

    ImGui::PopStyleColor(4);

    ImGui::SetCursorPosY(ImGui::GetWindowHeight() - 30);
    imguipp::center_text_ex("(c) SmartHouse", 230, 1, false);
  }

  ImGui::NextColumn();

  // Right side
  {
    // Dumper Tab
    if (settings::Tab == 3) {

      if (ImGui::Button("Get logs")) {
        SetLogs();
      }
      ImGui::SameLine();
      if (ImGui::Button("Clear logs")) {
        ClearLogs();
      }

      imguipp::center_text_ex(ICON_FA_INFO_CIRCLE "  Logs Information:", 230, 1,
                              false);

      ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(0, 0, 0, 0));
      if (ImGui::ListBoxHeader("##ResourcesList",
                               ImVec2(imguipp::getx(), imguipp::gety() - 35))) {
        for (const auto& resource : logs) {
          if (ImGui::TreeNode(resource.c_str())) ImGui::TreePop();
        }
        ImGui::ListBoxFooter();
      }
      ImGui::PopStyleColor();
    }
  }
}