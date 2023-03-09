#pragma once
#include "serverUI.h"

#define opened "opened"
#define closed "closed"
#define on "on"
#define off "off"
#define pushColorGreen \
  ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(ImColor(0, 250, 154)));
#define pushColorRed \
  ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(ImColor(250, 128, 114)));
#define pushColorBlue \
  ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(ImColor(78, 189, 221)));

void myUI::Update() {
  SetMenuTheme();
  ImGui::Begin("ServerUI", 0,
               ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize |
                   ImGuiWindowFlags_NoCollapse);

  ImGui::Columns(2);
  ImGui::SetColumnOffset(1, 230);

  RenderLeftSide();

  ImGui::NextColumn();
  RenderRightSide();

  ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(0, 0, 0, 0));
  ImGui::PopStyleColor();

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

void myUI::RenderLeftSide() {
  static ImVec4 active = imguipp::to_vec4(41, 40, 41, 255);
  static ImVec4 inactive = imguipp::to_vec4(31, 30, 31, 255);

  ImGui::PushStyleColor(ImGuiCol_Button,
                        settings::Tab == 1 ? active : inactive);
  pushColorBlue;
  if (ImGui::Button(ICON_FA_HOURGLASS_START " Run Server",
                    ImVec2(230 - 15, 41)))
    settings::Tab = 1;
  ImGui::PopStyleColor();

  ImGui::Spacing();

  ImGui::PushStyleColor(ImGuiCol_Button,
                        settings::Tab == 2 ? active : inactive);
  pushColorBlue;
  if (ImGui::Button(ICON_FA_CODE " Logs", ImVec2(230 - 15, 41)))
    settings::Tab = 2;
  ImGui::PopStyleColor();

  ImGui::Spacing();

  ImGui::PushStyleColor(ImGuiCol_Button,
                        settings::Tab == 3 ? active : inactive);
  pushColorBlue;
  if (ImGui::Button(ICON_FA_BOOK " Menu", ImVec2(230 - 15, 41)))
    settings::Tab = 3;
  ImGui::PopStyleColor();

  ImGui::PopStyleColor(3);
  ImGui::SetCursorPosY(ImGui::GetWindowHeight() - 30);
  imguipp::center_text_ex("(c) SmartHouse", 230, 1, false);
}

void myUI::RenderRightSide() {
  RenderRunServer();
  RenderLogs();
  RenderMenu();
}

void myUI::RenderRunServer() {
  if (settings::Tab == 1) {
    if (!isLocalHost) {
      pushColorGreen;
      if (ImGui::Button(ICON_FA_PLAY " Start Server")) {
        std::thread th([]() { connection::StartStopServer("/start"); });
        th.detach();
        isServerActive = true;
        SetStatuses();
      }
      ImGui::PopStyleColor();
      ImGui::SameLine();

      pushColorRed;
      if (ImGui::Button(ICON_FA_STOP " Stop Server")) {
        std::thread th([]() { connection::StartStopServer("/stop"); });
        th.detach();
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

      pushColorBlue;
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

      pushColorBlue;
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
}

void myUI::RenderLogs() {
  if (settings::Tab == 2 && isServerActive) {
    pushColorBlue;
    if (ImGui::Button(ICON_FA_FILE_CODE " All logs", ImVec2(130, 30))) {
      ClearLogs();
      std::thread th([this]() { logs = connection::UpdateLogs("", url); });
      th.detach();
    }
    ImGui::PopStyleColor();
    ImGui::SameLine();

    pushColorBlue;
    if (ImGui::Button(ICON_FA_INFO " Information", ImVec2(130, 30))) {
      ClearLogs();
      std::thread th(
          [this]() { logs = connection::UpdateLogs("information", url); });
      th.detach();
    }
    ImGui::PopStyleColor();
    ImGui::SameLine();

    pushColorBlue;
    if (ImGui::Button(ICON_FA_MAP_SIGNS " Error", ImVec2(130, 30))) {
      ClearLogs();
      std::thread th([this]() { logs = connection::UpdateLogs("error", url); });
      th.detach();
    }
    ImGui::PopStyleColor();
    ImGui::SameLine();

    pushColorBlue;
    if (ImGui::Button(ICON_FA_MAP_PIN " Warning", ImVec2(130, 30))) {
      std::thread th(
          [this]() { logs = connection::UpdateLogs("warning", url); });
      th.detach();
    }
    ImGui::PopStyleColor();
    ImGui::SameLine();

    pushColorBlue;
    if (ImGui::Button(ICON_FA_TRASH " Clear", ImVec2(130, 30))) {
      ClearLogs();
    }
    ImGui::PopStyleColor();
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
}

void myUI::RenderMenu() {
  if (settings::Tab == 3 && isServerActive) {
    if (!type.isWindows && !type.isLights && !type.isDevices) {
      pushColorBlue;
      if (ImGui::Button(ICON_FA_WINDOW_MAXIMIZE "  Windows", ImVec2(150, 35))) {
        type.isWindows = true;
      }
      ImGui::PopStyleColor();
      ImGui::SameLine();

      pushColorBlue;
      if (ImGui::Button(ICON_FA_LAPTOP "  Devices", ImVec2(150, 35))) {
        type.isDevices = true;
      }
      ImGui::PopStyleColor();
      ImGui::SameLine();

      pushColorBlue;
      if (ImGui::Button(ICON_FA_LIGHTBULB "  Lights", ImVec2(150, 35))) {
        type.isLights = true;
      }
      ImGui::PopStyleColor();
      ImGui::SameLine();

      // refresh status
      pushColorBlue;
      ImGui::SetCursorPosX(910);
      if (ImGui::Button(ICON_FA_REDO " Refresh statuses")) {
        SetStatuses();
      }
      ImGui::PopStyleColor();
      // refresh status ends
    }

    // windows start
    if (type.isWindows) {
      ClosedOpen(3);
      ImGui::SameLine();
      ClosedOpen(4);
      ImGui::SameLine();
      ClosedOpen(5);
      ClosedOpen(6);
      ImGui::SameLine();
      ClosedOpen(7);
      ImGui::SameLine();
      ClosedOpen(8);
      ClosedOpen(9);

      ImGui::NewLine();
      pushColorBlue;
      if (ImGui::Button("Back to menu")) type.isWindows = false;
      ImGui::PopStyleColor();
    }
    // winodws end

    // devices start
    if (type.isDevices) {
      OffOn(10);  // rice cooker
      ImGui::SameLine();
      OffOn(20);  // cooker
      ImGui::SameLine();
      OffOn(11);  // air conditioner bedroom
      ImGui::SameLine();
      OffOn(12);       // TV bedroom
      ClosedOpen(19);  // tap
      ImGui::SameLine();
      OffOn(14);  // TV hallwayy
      ImGui::SameLine();
      OffOn(15);  // air conditioner kidsroom
      ImGui::SameLine();
      OffOn(16);  // air conditioner boysroom
      OffOn(17);  // mac
      ImGui::SameLine();
      OffOn(18);  // laptop
      ImGui::SameLine();
      OffOn(13);  // air conditioner bathroom

      ImGui::NewLine();
      pushColorBlue;
      if (ImGui::Button("Back to menu")) type.isDevices = false;
      ImGui::PopStyleColor();
    }
    // devices end

    // lights start
    if (type.isLights) {
      OffOn(21);  // hallway
      ImGui::SameLine();
      OffOn(28);  // hallway
      ImGui::SameLine();
      OffOn(23);  // kitchen
      ImGui::SameLine();
      OffOn(24);  // starage
      OffOn(25);  // office
      ImGui::SameLine();
      OffOn(26);  // bathroom
      ImGui::SameLine();
      OffOn(32);  // bathroom
      ImGui::SameLine();
      OffOn(22);  // garage
      OffOn(29);  // boysroom
      ImGui::SameLine();
      OffOn(30);  // kidsroom
      ImGui::SameLine();
      OffOn(31);  // bedroom
      ImGui::SameLine();
      OffOn(27);  // basement
      OffOn(33);  // gym
      ImGui::SameLine();
      OffOn(34);  // wardrobe
      ImGui::SameLine();
      OffOn(35);  // lounge

      ImGui::NewLine();
      pushColorBlue;
      if (ImGui::Button("Back to menu")) type.isLights = false;
      ImGui::PopStyleColor();
    }
    // lights end
  }
}

void myUI::SetStatuses() {
  if (isServerActive) devices = connection::UpdateMenuStatuses(url);
}
void myUI::SetIsServerActive() {
  isServerActive = connection::IsServerActive(url);
}

void myUI::ClosedOpen(int id) {
  if (devices.size()) {
    const std::string NAME_closed =
        devices[id].name + "\nroom: " + devices[id].room + "\nCLOSE";
    const std::string NAME_opened =
        devices[id].name + "\nroom: " + devices[id].room + "\nOPEN";

    if (devices[id].status == opened) {
      pushColorRed;
      if (ImGui::Button(NAME_closed.c_str(), ImVec2(170, 0))) {
        std::thread th([this, id]() {
          connection::UpdateStatus("/device?id=" + devices[id].id, closed, url);
        });
        th.detach();
        devices[id].status = closed;
      }
      ImGui::PopStyleColor();
    } else {
      pushColorGreen;
      if (ImGui::Button(NAME_opened.c_str(), ImVec2(170, 0))) {
        std::thread th([this, id]() {
          connection::UpdateStatus("/device?id=" + devices[id].id, opened, url);
        });
        th.detach();
        devices[id].status = opened;
      }
      ImGui::PopStyleColor();
    }
  }
}

void myUI::OffOn(int id) {
  if (devices.size()) {
    const std::string NAME_off =
        devices[id].name + "\nroom: " + devices[id].room + "\nOFF";
    const std::string NAME_on =
        devices[id].name + "\nroom: " + devices[id].room + "\nON";

    if (devices[id].status == on) {
      pushColorRed;
      if (ImGui::Button(NAME_off.c_str(), ImVec2(170, 0))) {
        std::thread th([this, id]() {
          connection::UpdateStatus("/device?id=" + devices[id].id, off, url);
        });
        th.detach();
        devices[id].status = closed;
      }
      ImGui::PopStyleColor();
    } else {
      pushColorGreen;
      if (ImGui::Button(NAME_on.c_str(), ImVec2(170, 0))) {
        std::thread th([this, id]() {
          connection::UpdateStatus("/device?id=" + devices[id].id, on, url);
        });
        th.detach();
        devices[id].status = on;
      }
      ImGui::PopStyleColor();
    }
  }
}
