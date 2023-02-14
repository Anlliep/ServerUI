#pragma once
#include "serverUI.h"

void myUI::Update() {
  SetMenuTheme();
  ImGui::Begin("ServerUI", 0,
               ImGuiWindowFlags_NoScrollbar |
                   ImGuiWindowFlags_NoScrollWithMouse |
                   ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
  RenderMenu();

  ImGui::End();
  ImGui::EndFrame();
}

void myUI::SetMenuTheme() {
  ImGuiStyle* style = &ImGui::GetStyle();

  style->WindowBorderSize = 0;
  style->WindowTitleAlign = ImVec2(0.5, 0.5);
  style->WindowMinSize = ImVec2(1070, 470);

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
      if (!isLocalHost) {
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                              ImVec4(ImColor(0, 250, 154)));
        if (ImGui::Button(ICON_FA_PLAY " Start Server")) {
          connection::StartStopServer("/start");
          isServerActive = true;
          SetStatuses();
        }
        ImGui::PopStyleColor();
        ImGui::SameLine();
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                              ImVec4(ImColor(250, 128, 114)));
        if (ImGui::Button(ICON_FA_STOP " Stop Server")) {
          connection::StartStopServer("/stop");
          isServerActive = false;
        }
        ImGui::PopStyleColor();
        ImGui::NewLine();
        if (isServerActive)
          imguipp::center_text_ex(ICON_FA_INFO_CIRCLE "  Server is running!",
                                  230, 1, true);
        else
          imguipp::center_text_ex(ICON_FA_INFO_CIRCLE "  Server isn't active!",
                                  230, 1, true);
        ImGui::NewLine();
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                              ImVec4(ImColor(78, 189, 221)));
        if (ImGui::Button(ICON_FA_REDO " Local Host")) {
          url = "http://localhost:8080";
          SetIsServerActive();

          if (isServerActive) {
            isLocalHost = true;
            SetStatuses();
          } else {
            url = "http://15.188.57.7:8080";
          }
        }
        ImGui::PopStyleColor();
      }
      if (isLocalHost) {
        imguipp::center_text_ex(ICON_FA_INFO_CIRCLE "  localhost!", 230, 1,
                                true);

        ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                              ImVec4(ImColor(0, 250, 154)));
        if (ImGui::Button(ICON_FA_INFO_CIRCLE "go back")) {
          url = "http://15.188.57.7:8080";
          isLocalHost = false;
          SetIsServerActive();

          if (isServerActive) {
            SetStatuses();
          }
        }
        ImGui::PopStyleColor();
      }
    }

    if (settings::Tab == 2 && isServerActive) {
      if (ImGui::Button(ICON_FA_FILE_CODE " All logs")) {
        ClearLogs();
        logs = connection::UpdateLogs("", url);
      }
      ImGui::SameLine();
      if (ImGui::Button(ICON_FA_INFO " Information")) {
        ClearLogs();
        logs = connection::UpdateLogs("information", url);
      }
      ImGui::SameLine();
      if (ImGui::Button(ICON_FA_MAP_SIGNS " Error")) {
        ClearLogs();
        logs = connection::UpdateLogs("error", url);
      }
      ImGui::SameLine();
      if (ImGui::Button(ICON_FA_MAP_PIN " Warning")) {
        ClearLogs();
        logs = connection::UpdateLogs("warning", url);
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
      ImGui::Text(ICON_FA_LIGHTBULB " Light: ");
      ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                            ImVec4(ImColor(0, 250, 154)));
      if (ImGui::Button("On##1", ImVec2(70, 0))) {
        connection::UpdateStatus("/device?id=3", "on", url);
        menuStatuses.isLight = true;
      }
      ImGui::PopStyleColor();
      ImGui::SameLine();
      ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                            ImVec4(ImColor(250, 128, 114)));
      if (ImGui::Button("Off##1", ImVec2(70, 0))) {
        connection::UpdateStatus("/device?id=3", "off", url);
        menuStatuses.isLight = false;
      }
      ImGui::PopStyleColor();
      ImGui::SameLine();

      if (menuStatuses.isLight)
        ImGui::Text(ICON_FA_INFO_CIRCLE "  Light is on");
      else
        ImGui::Text(ICON_FA_INFO_CIRCLE "  Light is off");

      ImGui::NewLine();
      ImGui::Text(ICON_FA_LAPTOP " Device: ");
      ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                            ImVec4(ImColor(0, 250, 154)));
      if (ImGui::Button("On##2", ImVec2(70, 0))) {
        connection::UpdateStatus("/device?id=4", "on", url);
        menuStatuses.isDevice = true;
      }
      ImGui::PopStyleColor();
      ImGui::SameLine();
      ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                            ImVec4(ImColor(250, 128, 114)));
      if (ImGui::Button("Off##2", ImVec2(70, 0))) {
        connection::UpdateStatus("/device?id=4", "off", url);
        menuStatuses.isDevice = false;
      }
      ImGui::PopStyleColor();
      ImGui::SameLine();

      if (menuStatuses.isDevice)
        ImGui::Text(ICON_FA_INFO_CIRCLE "  Device is on");
      else
        ImGui::Text(ICON_FA_INFO_CIRCLE "  Device is off");

      ImGui::NewLine();
      ImGui::Text(ICON_FA_FIRE " Oven: ");
      ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                            ImVec4(ImColor(0, 250, 154)));
      if (ImGui::Button("On##3", ImVec2(70, 0))) {
        connection::UpdateStatus("/device?id=5", "on", url);
        menuStatuses.isOven = true;
      }
      ImGui::PopStyleColor();
      ImGui::SameLine();
      ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                            ImVec4(ImColor(250, 128, 114)));
      if (ImGui::Button("Off##3", ImVec2(70, 0))) {
        connection::UpdateStatus("/device?id=5", "off", url);
        menuStatuses.isOven = false;
      }
      ImGui::PopStyleColor();
      ImGui::SameLine();

      if (menuStatuses.isOven)
        ImGui::Text(ICON_FA_INFO_CIRCLE "  Oven is on");
      else
        ImGui::Text(ICON_FA_INFO_CIRCLE "  Oven is off");

      ImGui::NewLine();
      ImGui::Text(ICON_FA_WINDOW_MAXIMIZE " Windows: ");
      ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                            ImVec4(ImColor(0, 250, 154)));
      if (ImGui::Button("Open##1", ImVec2(70, 0))) {
        connection::UpdateStatus("/device?id=2", "opened", url);
        menuStatuses.isWindows = true;
      }
      ImGui::PopStyleColor();
      ImGui::SameLine();
      ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                            ImVec4(ImColor(250, 128, 114)));
      if (ImGui::Button("Close##1", ImVec2(70, 0))) {
        connection::UpdateStatus("/device?id=2", "closed", url);
        menuStatuses.isWindows = false;
      }
      ImGui::PopStyleColor();
      ImGui::SameLine();
      if (menuStatuses.isWindows)
        ImGui::Text(ICON_FA_INFO_CIRCLE "  Windows are opened");
      else
        ImGui::Text(ICON_FA_INFO_CIRCLE "  Windows are closed");
      ImGui::NewLine();

      ImGui::Text(ICON_FA_WATER ICON_FA_WATER_LOWER ICON_FA_WATER_RISE
                  " Tap: ");
      ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                            ImVec4(ImColor(0, 250, 154)));
      if (ImGui::Button("Open##2", ImVec2(70, 0))) {
        connection::UpdateStatus("/device?id=6", "opened", url);
        menuStatuses.isTap = true;
      }
      ImGui::PopStyleColor();
      ImGui::SameLine();
      ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                            ImVec4(ImColor(250, 128, 114)));
      if (ImGui::Button("Close##2", ImVec2(70, 0))) {
        connection::UpdateStatus("/device?id=6", "closed", url);
        menuStatuses.isTap = false;
      }
      ImGui::PopStyleColor();
      ImGui::SameLine();

      if (menuStatuses.isTap)
        ImGui::Text(ICON_FA_INFO_CIRCLE "  Tap is opened");
      else
        ImGui::Text(ICON_FA_INFO_CIRCLE "  Tap is closed");
      ImGui::NewLine();

      ImGui::Text(ICON_FA_CAR " Garage door: ");
      ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                            ImVec4(ImColor(0, 250, 154)));
      if (ImGui::Button("Open##3", ImVec2(70, 0))) {
        connection::UpdateStatus("/device?id=7", "opened", url);
        menuStatuses.isGarage = true;
      }
      ImGui::PopStyleColor();
      ImGui::SameLine();
      ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                            ImVec4(ImColor(250, 128, 114)));
      if (ImGui::Button("Close##3", ImVec2(70, 0))) {
        connection::UpdateStatus("/device?id=7", "closed", url);
        menuStatuses.isGarage = false;
      }
      ImGui::PopStyleColor();
      ImGui::SameLine();

      if (menuStatuses.isGarage)
        ImGui::Text(ICON_FA_INFO_CIRCLE "  Garage door is opened");
      else
        ImGui::Text(ICON_FA_INFO_CIRCLE "  Garage door is closed");
      ImGui::NewLine();
    }
  }
  ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(0, 0, 0, 0));
  ImGui::PopStyleColor();
}

void myUI::SetStatuses() {
  if (isServerActive) menuStatuses = connection::UpdateMenuStatuses(url);
}
void myUI::SetIsServerActive() {
  isServerActive = connection::IsServerActive(url);
}