#pragma once
#include "serverUI.h"

void myUI::Update() {
  SetMenuTheme();
  ImGui::Begin(
      "ServerUI", 0,
      ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
  RenderMenu();

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
    if (ImGui::Button(ICON_FA_CODE " Logs", ImVec2(230 - 15, 41)))
      settings::Tab = 2;

    ImGui::Spacing();
    ImGui::PushStyleColor(ImGuiCol_Button,
                          settings::Tab == 3 ? active : inactive);
    if (ImGui::Button(ICON_FA_BOOK " Menu", ImVec2(230 - 15, 41)))
      settings::Tab = 3;

    ImGui::PopStyleColor(3);

    ImGui::SetCursorPosY(ImGui::GetWindowHeight() - 30);
    imguipp::center_text_ex("(c) SmartHouse", 230, 1, false);
  }

  ImGui::NextColumn();
  // Right side
  {
    if (settings::Tab == 1) {
      ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                            ImVec4(ImColor(0, 250, 154)));
      if (ImGui::Button(ICON_FA_PLAY " Start Server")) {
        if (strlen(buf) == 0) {
          connection::StartStopServer("http://15.188.57.7:8008", "/start");
        } else {
          connection::StartStopServer(buf, "/start");
        }
        isServerActive = true;
      }
      ImGui::PopStyleColor();
      ImGui::SameLine();
      ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                            ImVec4(ImColor(250, 128, 114)));
      if (ImGui::Button(ICON_FA_STOP " Stop Server")) {
        connection::StartStopServer("http://15.188.57.7:8008", "/stop");
        isServerActive = false;
      }
      ImGui::PopStyleColor();
      ImGui::NewLine();
      if (isServerActive)
        imguipp::center_text_ex(ICON_FA_INFO_CIRCLE "  Server is running!", 230,
                                1, true);
      else
        imguipp::center_text_ex(ICON_FA_INFO_CIRCLE "  Server isn't active!",
                                230, 1, true);
      ImGui::NewLine();
      imguipp::center_text_ex(
          ICON_FA_INFO_CIRCLE " Determine your server address:", 245, 1, true);
      ImGui::PushItemWidth(240.0f);
      /*ImGui::PushStyleColor(ImGuiCol_FrameBg,
        ImVec4(ImColor(0, 250, 154)));*/
      ImGui::InputTextWithHint("##input", "http://ip:port", buf, size,
                               ImGuiInputTextFlags_CharsNoBlank);
      /*ImGui::PopStyleColor();*/
      ImGui::PopItemWidth();
    }

    if (settings::Tab == 2 && isServerActive) {
      if (ImGui::Button(ICON_FA_FILE_CODE " All logs")) {
        ClearLogs();
        logs = connection::UpdateLogs("");
      }
      ImGui::SameLine();
      if (ImGui::Button(ICON_FA_INFO " Information")) {
        ClearLogs();
        logs = connection::UpdateLogs("information");
      }
      ImGui::SameLine();
      if (ImGui::Button(ICON_FA_MAP_SIGNS " Error")) {
        ClearLogs();
        logs = connection::UpdateLogs("error");
      }
      ImGui::SameLine();
      if (ImGui::Button(ICON_FA_MAP_PIN " Warning")) {
        ClearLogs();
        logs = connection::UpdateLogs("warning");
      }
      ImGui::SameLine();
      if (ImGui::Button(ICON_FA_TRASH " Clear")) {
        ClearLogs();
      }

      ImGui::NewLine();
      imguipp::center_text_ex(ICON_FA_INFO_CIRCLE "  Logs Information:", 230, 1,
                              false);

      ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(0, 0, 0, 0));
      if (ImGui::ListBoxHeader("##ResourcesList",
                               ImVec2(imguipp::getx(), imguipp::gety() - 35))) {
        for (auto it = logs.crbegin(); it != logs.crend(); ++it) {
          std::string logsStr{*it};
          if (ImGui::TreeNode(logsStr.c_str())) ImGui::TreePop();
        }
        ImGui::ListBoxFooter();
      }
      ImGui::PopStyleColor();
    }

    if (settings::Tab == 3 && isServerActive) {
      ImGui::Text(ICON_FA_INFO_CIRCLE " Light: ");

      if (ImGui::Button("On##1", ImVec2(70, 0))) {
        connection::UpdateStatus("/device?id=3", "on");
      }
      ImGui::SameLine();

      if (ImGui::Button("Off##1", ImVec2(70, 0))) {
        connection::UpdateStatus("/device?id=3", "off");
      }
      ImGui::NewLine();

      ImGui::Text(ICON_FA_INFO_CIRCLE " Device: ");

      if (ImGui::Button("On##2", ImVec2(70, 0))) {
        connection::UpdateStatus("/device?id=4", "on");
      }
      ImGui::SameLine();

      if (ImGui::Button("Off##2", ImVec2(70, 0))) {
        connection::UpdateStatus("/device?id=4", "off");
      }
      ImGui::NewLine();

      ImGui::Text(ICON_FA_INFO_CIRCLE " Oven: ");

      if (ImGui::Button("On##3", ImVec2(70, 0))) {
        connection::UpdateStatus("/device?id=5", "on");
      }
      ImGui::SameLine();

      if (ImGui::Button("Off##3", ImVec2(70, 0))) {
        connection::UpdateStatus("/device?id=5", "off");
      }
      ImGui::NewLine();

      ImGui::Text(ICON_FA_INFO_CIRCLE " Windows: ");

      if (ImGui::Button("Open##1", ImVec2(70, 0))) {
        connection::UpdateStatus("/device?id=2", "opened");
      }
      ImGui::SameLine();

      if (ImGui::Button("Close##1", ImVec2(70, 0))) {
        connection::UpdateStatus("/device?id=2", "closed");
      }
      ImGui::NewLine();

      ImGui::Text(ICON_FA_INFO_CIRCLE " Tap: ");

      if (ImGui::Button("Open##2", ImVec2(70, 0))) {
        connection::UpdateStatus("/device?id=6", "opened");
      }
      ImGui::SameLine();

      if (ImGui::Button("Close##2", ImVec2(70, 0))) {
        connection::UpdateStatus("/device?id=6", "closed");
      }
      ImGui::NewLine();

      ImGui::Text(ICON_FA_INFO_CIRCLE " Garage door: ");

      if (ImGui::Button("Open##3", ImVec2(70, 0))) {
        connection::UpdateStatus("/device?id=7", "opened");
      }
      ImGui::SameLine();

      if (ImGui::Button("Close##3", ImVec2(70, 0))) {
        connection::UpdateStatus("/device?id=7", "closed");
      }
    }
  }
  ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(0, 0, 0, 0));
  ImGui::PopStyleColor();
}
