#include "menu.h"
#include "icons.h"
#include "imguipp.h"
#include "settings.h"

void Menu::Render() {
  ImGui::Columns(2);
  ImGui::SetColumnOffset(1, 230);

  {
    // Left side

    static ImVec4 active = imguipp::to_vec4(41, 40, 41, 255);
    static ImVec4 inactive = imguipp::to_vec4(31, 30, 31, 255);

    ImGui::PushStyleColor(ImGuiCol_Button,
                          Settings::Tab == 1 ? active : inactive);
    if (ImGui::Button(ICON_FA_HOURGLASS_START " Run Server",
                      ImVec2(230 - 15, 41)))
      Settings::Tab = 1;

    ImGui::Spacing();
    ImGui::PushStyleColor(ImGuiCol_Button,
                          Settings::Tab == 2 ? active : inactive);
    if (ImGui::Button(ICON_FA_USERS " Users", ImVec2(230 - 15, 41)))
      Settings::Tab = 2;

    ImGui::Spacing();
    ImGui::PushStyleColor(ImGuiCol_Button,
                          Settings::Tab == 3 ? active : inactive);
    if (ImGui::Button(ICON_FA_CODE " Logs", ImVec2(230 - 15, 41)))
      Settings::Tab = 3;

    ImGui::Spacing();
    ImGui::PushStyleColor(ImGuiCol_Button,
                          Settings::Tab == 4 ? active : inactive);
    if (ImGui::Button(ICON_FA_BOOK " Menu", ImVec2(230 - 15, 41)))
      Settings::Tab = 4;      

    ImGui::PopStyleColor(4);

    ImGui::SetCursorPosY(ImGui::GetWindowHeight() - 30);
    imguipp::center_text_ex("(c) SmartHouse", 230, 1, false);
  }

  ImGui::NextColumn();

  // Right side
  {
    // Dumper Tab
    if (Settings::Tab == 3) {
      std::vector<std::string> resources;

      for (int i = 0; i < 25; i++) {
        resources.push_back("2023-02-02 19:06 Server has been started");
      }

      imguipp::center_text_ex(ICON_FA_INFO_CIRCLE "  Logs Information:", 230, 1,
                              false);

      ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(0, 0, 0, 0));
      if (ImGui::ListBoxHeader("##ResourcesList",
                               ImVec2(imguipp::getx(), imguipp::gety() - 35))) {
        for (const auto& resource : resources) {
          if (ImGui::TreeNode(resource.c_str())) ImGui::TreePop();
        }
        ImGui::ListBoxFooter();
      }
      ImGui::PopStyleColor();
    }
  }
}

void Menu::Theme() {
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