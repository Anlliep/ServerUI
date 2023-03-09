#pragma once
#include <stdio.h>

#include <ctime>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "../bindings/imgui_impl_glfw.h"
#include "../bindings/imgui_impl_opengl3.h"
#include "icons.h"
#include "imgui.h"
#include "imguipp.h"
#include "serverRequests.h"

#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h>  // Will drag system OpenGL headers

#if defined(_MSC_VER) && (_MSC_VER >= 1900) && \
    !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

static void glfw_error_callback(int error, const char* description) {
  fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

namespace settings {
static int Tab = 1;
}  // namespace settings

template <typename Derived>
class UI {
 public:
  UI() {
    // Setup window
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) std::exit(1);

      // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    const char* glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+
    // only glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // 3.0+ only
#endif

    // Create window with graphics context
    window = glfwCreateWindow(1100, 500, "UI", NULL, NULL);
    if (window == NULL) std::exit(1);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);  // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;

    // your fonts here
    io.Fonts->AddFontFromFileTTF("../fonts/InsightSansSSi.ttf", 16.0f);
    // fonts end

    // your icons here
    ImFontConfig config;
    config.MergeMode = true;
    config.GlyphMinAdvanceX =
        13.0f;  // Use if you want to make the icon monospaced
    static const ImWchar icon_ranges[] = {ICON_MIN_FA, ICON_MAX_FA, 0};
    io.Fonts->AddFontFromFileTTF("../fonts/fontawesome-webfont.ttf", 13.0f,
                                 &config, icon_ranges);
    // icons end

    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable
    // Keyboard Controls io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; //
    // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    // ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Our state
  }
  ~UI() {
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
  }

  void Run() {
    StartUp();

    while (!glfwWindowShouldClose(window)) {
      glfwPollEvents();

      // Start the Dear ImGui frame
      ImGui_ImplOpenGL3_NewFrame();
      ImGui_ImplGlfw_NewFrame();
      ImGui::NewFrame();

      Update();

      // Rendering
      ImGui::Render();
      int display_w, display_h;
      glfwGetFramebufferSize(window, &display_w, &display_h);
      glViewport(0, 0, display_w, display_h);
      glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w,
                   clear_color.z * clear_color.w, clear_color.w);
      glClear(GL_COLOR_BUFFER_BIT);
      ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

      glfwSwapBuffers(window);
    }
  }
  void Update() { static_cast<Derived*>(this)->Update(); }
  void StartUp() { static_cast<Derived*>(this)->StartUp(); }

 protected:
  GLFWwindow* window;
  ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
};

class myUI : public UI<myUI> {
 public:
  myUI() {}
  ~myUI() = default;
  void StartUp() {}
  void Update();

  void SetMenuTheme();

  void RenderLeftSide();
  void RenderRightSide();
  void RenderRunServer();
  void RenderLogs();
  void RenderMenu();

  void ClearLogs() { logs.clear(); }
  void SetIsServerActive();
  void SetStatuses();
  void ClosedOpen(int id);
  void OffOn(int id);

 private:
  struct isType {
    bool isWindows{};
    bool isDevices{};
    bool isLights{};
  };

  isType type;
  std::vector<std::string> logs{};
  bool isServerActive{};
  bool isLocalHost = false;
  std::string url = "http://15.188.57.7:8080";
  std::vector<connection::Devices> devices{};
};
