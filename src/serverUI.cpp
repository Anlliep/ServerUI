#pragma once
#include "serverUI.h"

#define opened "opened"
#define closed "closed"
#define on "on"
#define off "off"

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
        if (ImGui::Button(ICON_FA_REDO " Local Host", ImVec2(123, 0))) {
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
        imguipp::center_text_ex(ICON_FA_INFO_CIRCLE "  Local host is running!",
                                230, 1, true);
        ImGui::NewLine();

        ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                              ImVec4(ImColor(0, 250, 154)));
        if (ImGui::Button(ICON_FA_BACKWARD " Go back")) {
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
      if (!type.isWindows && !type.isLights && !type.isDevices) {
        if (ImGui::Button("Windows managment")) {
          type.isWindows = true;
        }
        ImGui::SameLine();

        if (ImGui::Button("Devices managment")) {
          type.isDevices = true;
        }
        ImGui::SameLine();

        if (ImGui::Button("Lights managment")) {
          type.isLights = true;
        }

        // refresh status
        ImGui::SameLine();
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                              ImVec4(ImColor(78, 189, 221)));
        ImGui::SetCursorPosX(915);
        if (ImGui::Button("Refresh statuses")) {
          SetStatuses();
        }
        ImGui::PopStyleColor();
        // refresh status ends
      }

      // windows start
      if (type.isWindows) {
        ClosedOpen("kitchenWindow");
        ClosedOpen("officeWindow");
        ClosedOpen("boysRoomWindow");
        ClosedOpen("kidsRoomSmallWindow");
        ClosedOpen("kidsRoomBigWindow");
        ClosedOpen("bedRoomWindow");
        ClosedOpen("secondFloorSmallWindow");

        ImGui::NewLine();
        if (ImGui::Button("Back to menu")) type.isWindows = false;
      }
      // winodws end

      // devices start
      if (type.isDevices) {
        OffOn("riceCooker");
        OffOn("bedroomAirConditioner");
        OffOn("bedroomTV");
        OffOn("secondFloorBathAirConditioner");
        OffOn("secondFloorTV");
        OffOn("kidsRoomAirConditioner");
        OffOn("boysRoomAirConditioner");
        OffOn("mac");
        OffOn("laptop");
        OffOn("cooker");
        OffOn("tap");

        ImGui::NewLine();
        if (ImGui::Button("Back to menu")) type.isDevices = false;
      }
      // devices end

      // lights start
      if (type.isLights) {
        OffOn("hallwayLights");
        ImGui::SameLine();
        OffOn("garageLights");
        ImGui::SameLine();
        OffOn("kitchenLights");
        OffOn("storageLights");
        ImGui::SameLine();
        OffOn("officeLights");
        ImGui::SameLine();
        OffOn("firstFloorBathroomLights");
        OffOn("basementLights");
        ImGui::SameLine();
        OffOn("secondFloorLights");
        OffOn("boysRoomLights");
        ImGui::SameLine();
        OffOn("kidsRoomLights");
        OffOn("bedroomLights");
        ImGui::SameLine();
        OffOn("secondFloorBathroomLights");
        OffOn("gymLights");
        ImGui::SameLine();
        OffOn("wardrobeLights");
        ImGui::SameLine();
        OffOn("loungeLights");

        ImGui::NewLine();
        if (ImGui::Button("Back to menu")) type.isLights = false;
      }
      // lights end
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

void myUI::OffOn(std::string name) {
  const std::string NAME_off = "Off " + name;
  const std::string NAME_on = "On " + name;

  if (menuStatuses[name] == on) {
    if (ImGui::Button(NAME_off.c_str())) {
      connection::UpdateStatus("/device?name=" + name, off, url);
      menuStatuses[name] = off;
    }
  } else {
    if (ImGui::Button(NAME_on.c_str())) {
      connection::UpdateStatus("/device?name=" + name, on, url);
      menuStatuses[name] = on;
    }
  }
}

void myUI::ClosedOpen(std::string name) {
  const std::string NAME_closed = "Close " + name;
  const std::string NAME_opened = "Open " + name;

  if (menuStatuses[name] == opened) {
    if (ImGui::Button(NAME_closed.c_str())) {
      connection::UpdateStatus("/device?name=" + name, closed, url);
      menuStatuses[name] = closed;
    }
  } else {
    if (ImGui::Button(NAME_opened.c_str())) {
      connection::UpdateStatus("/device?name=" + name, opened, url);
      menuStatuses[name] = opened;
    }
  }
}