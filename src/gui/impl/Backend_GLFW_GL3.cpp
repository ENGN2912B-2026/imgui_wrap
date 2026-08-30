//  Copyright (c) 2024-2026 Daniel Moreno. All rights reserved.
//
#include "Backend_GLFW_GL3.hpp"


#ifdef USE_GLAD
# include <glad/gl.h>
#endif

#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include <GLFW/glfw3.h>

#ifdef USE_ROBOTO_WEBFONT
# include <roboto_regular_webfont_ttf.hpp>
#endif

#include <cmath>
#include <cstdio>

#ifdef __linux__
# include <unistd.h> // sleep
#endif

namespace
{
  float GetDPI_(GLFWmonitor* monitor)
  {
    float x_scale, y_scale;
    glfwGetMonitorContentScale(monitor, &x_scale, &y_scale);
    return x_scale;
  }

  float GetDPI_(GLFWwindow* window)
  {
#if GLFW_HAS_MONITOR_WORK_AREA
    int window_x, window_y, window_width, window_height;
    glfwGetWindowPos(window, &window_x, &window_y);
    glfwGetWindowSize(window, &window_width, &window_height);
    // Find window center
    window_x += (int)((float)window_width * 0.5f);
    window_y += (int)((float)window_height * 0.5f);
    int num_monitors = 0;
    GLFWmonitor** monitors = glfwGetMonitors(&num_monitors);
    while (num_monitors-- > 0)
    {
      GLFWmonitor* monitor = *monitors;
      int monitor_x, monitor_y, monitor_width, monitor_height;
      glfwGetMonitorWorkarea(monitor, &monitor_x, &monitor_y, &monitor_width, &monitor_height);
      if (monitor_x <= window_x && window_x < (monitor_x + monitor_width) && monitor_y <= window_y && window_y < (monitor_y + monitor_height))
        return ImGuiApp_ImplGlfw_GetDPI(monitor);
      monitors++;
    }
#else
    IM_UNUSED(window);
#endif
    return 1.0f;
  }

  bool CaptureFramebuffer_(
    ImGuiViewport* viewport,
    int x, int y, int w, int h,
    unsigned int* pixels, void* user_data)
  {
    IM_UNUSED(user_data);
    IM_UNUSED(viewport); // Expecting calling code to have set the right GL context

#ifdef __linux__
    // FIXME: Odd timing issue is observed on linux (Plasma/X11 specifically), which causes outdated frames to be captured, unless we give compositor some time to update screen.
    // glFlush() didn't seem enough. Will probably need to revisit that.
    usleep(1000);   // 1ms
#endif

    int y2 = static_cast<int>(ImGui::GetIO().DisplaySize.y) - (y + h);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glReadPixels(x, y2, w, h, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

    // Flip vertically
    size_t comp = 4;
    size_t stride = static_cast<size_t>(w) * comp;
    unsigned char* line_tmp = new unsigned char[stride];
    unsigned char* line_a = reinterpret_cast<unsigned char*>(pixels);
    unsigned char* line_b = line_a + (stride * (static_cast<size_t>(h) - 1));
    while (line_a < line_b)
    {
      memcpy(line_tmp, line_a, stride);
      memcpy(line_a, line_b, stride);
      memcpy(line_b, line_tmp, stride);
      line_a += stride;
      line_b -= stride;
    }
    delete[] line_tmp;
    return true;
  }

  void ErrorCallback_(int error, const char* description)
  {
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
  }
}

namespace gui
{
  std::unique_ptr<Backend_GLFW_GL3> Backend_GLFW_GL3::create()
  {
    return std::make_unique<Backend_GLFW_GL3>();
  }

   bool Backend_GLFW_GL3::InitCreateWindow(
    const char* window_title, ImVec2 window_size)
  {
    // The code in this function is based on the  example
    // `examples/example_glfw_opengl3/main.cpp` from the ImGui library.
    //
    // Review that code from time to time to ensure that this code is up to date
    // with the latest ImGui version.
    //

    // Setup window
    glfwSetErrorCallback(ErrorCallback_);
    if (!glfwInit())
    {
      return false;
    }

    // Decide GL+GLSL versions
#if __APPLE__
    // GL 3.2 + GLSL 150
    glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
    // GL 3.1 + GLSL 140
    glsl_version = "#version 140";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

    // Create window with graphics context
    GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
    DpiScale = DpiAware ? GetDPI_(primaryMonitor) : 1.0f;
    window_size.x = std::floor(window_size.x * DpiScale);
    window_size.y = std::floor(window_size.y * DpiScale);
    window = glfwCreateWindow(
      static_cast<int>(window_size.x), static_cast<int>(window_size.y),
      window_title, nullptr, nullptr);
    if (window == nullptr)
    {
      return false;
    }
    glfwMakeContextCurrent(window);

  #ifdef USE_GLAD
    // Load OpenGL and verify
    int version = gladLoadGL(glfwGetProcAddress);
    if (version == 0) {
        printf("Failed to initialize OpenGL context\n");
        return false;
    }
    // Successfully loaded OpenGL
    printf("GLAD: Loaded OpenGL %d.%d\n",
      GLAD_VERSION_MAJOR(version),
      GLAD_VERSION_MINOR(version));
  #endif

    // Adjust scale
    ImGuiIO& io = ImGui::GetIO();
    ImFontConfig font_config;
    font_config.OversampleH = 1;
    font_config.OversampleV = 1;
    font_config.PixelSnapH = true;
    font_config.SizePixels = 17.0f * DpiScale;
    font_config.GlyphOffset.y = 1.0f * DpiScale;
    io.Fonts->Clear();

#ifdef USE_ROBOTO_WEBFONT
    // Load Roboto webfont
    font_config.FontDataOwnedByAtlas = false;
    io.Fonts->AddFontFromMemoryTTF(
      roboto_regular_webfont_ttf_data,
      roboto_regular_webfont_ttf_size,
      font_config.SizePixels,
      &font_config);
#else
    // Load default font
    io.Fonts->AddFontDefault(&font_config);
#endif

    ImGui::GetStyle().ScaleAllSizes(DpiScale);

    return true;
  }

  void Backend_GLFW_GL3::InitBackends()
  {
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Display GL Info
    printf("OpenGL vendor: %s\n", glGetString(GL_VENDOR));
    printf("OpenGL renderer: %s\n", glGetString(GL_RENDERER));
    printf("OpenGL version: %s\n", glGetString(GL_VERSION));
    printf("GLSL version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
  }

  bool Backend_GLFW_GL3::NewFrame()
  {
    // Poll and handle events (inputs, window resize, etc.)
    // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to
    // tell if dear imgui wants to use your inputs.
    // - When io.WantCaptureMouse is true, do not dispatch mouse input data to
    //   your main application.
    // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input
    //   data to your main application.
    // Generally you may always pass all inputs to dear imgui, and hide them
    // from your application based on those two flags.
    glfwMakeContextCurrent(window);
    glfwPollEvents();
    if (glfwWindowShouldClose(window))
      return false;
    DpiScale = GetDPI_(window);
#if !defined(IMGUI_IMPL_OPENGL_ES3) && !defined(IMGUI_IMPL_OPENGL_ES2)
    if (SrgbFramebuffer)
      glEnable(GL_FRAMEBUFFER_SRGB);
    else
      glDisable(GL_FRAMEBUFFER_SRGB);
#endif
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    return true;
  }

  void Backend_GLFW_GL3::Render()
  {
    ImGuiIO& io = ImGui::GetIO();
    glfwMakeContextCurrent(window);
    glfwSwapInterval(Vsync ? 1 : 0);
    glViewport(
      0, 0,
      static_cast<int>(io.DisplaySize.x), static_cast<int>(io.DisplaySize.y));
    glClearColor(ClearColor.x, ClearColor.y, ClearColor.z, ClearColor.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(window);
  }

  void Backend_GLFW_GL3::ShutdownCloseWindow()
  {
    glfwDestroyWindow(window);
    glfwTerminate(); //would destroy ALL windows
  }

  void Backend_GLFW_GL3::ShutdownBackends()
  {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
  }

  bool Backend_GLFW_GL3::CaptureFramebuffer(
    ImGuiViewport* viewport,
    int x, int y, int w, int h,
    unsigned int* pixels, void* user_data)
  {
    return CaptureFramebuffer_(
      viewport, x, y, w, h, pixels, user_data);
  }

  void Backend_GLFW_GL3::SetWindowTitle(const char* title)
  {
    glfwSetWindowTitle(window, title);
  }

  void Backend_GLFW_GL3::SetWindowSize(const ImVec2& size)
  {
    glfwSetWindowSize(window, static_cast<int>(size.x), static_cast<int>(size.y));
  }

} // namespace gui
