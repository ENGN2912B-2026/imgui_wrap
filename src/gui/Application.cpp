//  Copyright (c) 2024 Daniel Moreno. All rights reserved.
//

#include <gui/Application.hpp>

#ifdef USE_GUI_TEST_ENGINE
  #include <gui/TestManager.hpp>
  #include "impl/Backend_Null.hpp"
  #include "impl/TestReporter.hpp"
  #include <imgui_te_engine.h>
  #include <imgui_te_ui.h>
  #include <imgui_te_utils.h>  // ImOsIsDebuggerPresent()
  #include <imgui_te_exporters.h>
  #include <iostream>
  #include <fstream>
  namespace
  {
    static ImGuiTestEngine* initTestEngine_()
    {
      // Setup test engine
      ImGuiTestEngine* engine = ImGuiTestEngine_CreateContext();
      ImGuiTestEngineIO& test_io = ImGuiTestEngine_GetIO(engine);
      test_io.ConfigVerboseLevel = ImGuiTestVerboseLevel_Info;
      test_io.ConfigVerboseLevelOnError = ImGuiTestVerboseLevel_Debug;
      //test_io.ConfigRunSpeed = ImGuiTestRunSpeed_Cinematic; // Set to cinematic for debugging
      test_io.ConfigNoThrottle = true; // Disable throttling by default
      //test_io.ScreenCaptureFunc = ImGuiApp_ScreenCaptureFunc;
      //test_io.ScreenCaptureUserData = (void*)app;
      test_io.ConfigLogToTTY = true;
      test_io.ConfigVerboseLevelOnError = ImGuiTestVerboseLevel_Warning;

      // Optional: save test output in junit-compatible XML format.
      //test_io.ExportResultsFile = "./results.xml";
      //test_io.ExportResultsFormat = ImGuiTestEngineExportFormat_JUnitXml;

      // Start test engine
      ImGuiTestEngine_Start(engine, ImGui::GetCurrentContext());
      ImGuiTestEngine_InstallDefaultCrashHandler();

      // Register tests
      gui::TestManager::getInstance().processRegistrations(engine);

      ImGuiTestEngine_QueueTests(engine, ImGuiTestGroup_Tests, "all");

      return engine;
    }
  }
#endif


namespace gui
{
  // static member initialization
  Application* Application::instance_{ nullptr };

  Application::Application(const std::string& title, const Vec2i& windowSize) :
    window_{ std::make_unique<Window>(title, windowSize) }
  {
    if (instance_ == nullptr)
    {
      setInstancePtr(this);
    }
  }

  Application::~Application()
  {
  }

  Window& Application::getWindow()
  {
    return *window_;
  }

  const Window& Application::getWindow() const
  {
    return *window_;
  }

  void Application::run()
  {
    //init window
    window_->init();

  #ifdef USE_GUI_TEST_ENGINE
    ImGuiTestEngine* engine = initTestEngine_();
  #endif

    // main loop
    running_ = true;
    while (running_ && window_->renderBegin())
    {
      window_->render();
#if defined(USE_GUI_TEST_ENGINE) && defined(SHOW_TEST_ENGINE_WINDOWS)
      ImGuiTestEngine_ShowTestEngineWindows(engine, NULL);
#endif
      window_->renderEnd();

#ifdef USE_GUI_TEST_ENGINE
      // Call after your rendering. This is mostly to support screen/video
      //  capturing features.
      ImGuiTestEngine_PostSwap(engine);

      // Exit after running all tests if backend is null,
      // or if GUI_EXIT_AFTER_TESTING is set to 1
      const bool GUI_EXIT_AFTER_TESTING{
        std::getenv("GUI_EXIT_AFTER_TESTING") != nullptr
        && std::string{ std::getenv("GUI_EXIT_AFTER_TESTING") } == "1" };
      const bool USE_NULL_BACKEND{
        dynamic_cast<Backend_Null*>(window_->getBackendPtr()) != nullptr };
      if (ImGuiTestEngine_IsTestQueueEmpty(engine)
        && (GUI_EXIT_AFTER_TESTING || USE_NULL_BACKEND))
      {
        break;
      }
#endif
    }

  #ifdef USE_GUI_TEST_ENGINE
    ImGuiTestEngine_Stop(engine);
  #endif

    //deinit window
    window_->deinit();

  #ifdef USE_GUI_TEST_ENGINE
    { // Print test results
      auto reporter = TestReporter::makeDefault();
      reporter->generateReport(engine, std::cout);
    }

    { // Create a markdown report
      const char* filename{ std::getenv("GUI_TEST_MARKDOWN_REPORT_FILE") };
      if (filename)
      {
        std::ofstream os{ filename };
        auto reporter = std::make_unique<TestReporterMarkdown>();
        reporter->generateReport(engine, os);
      }
    }

    // Get test results
    ImGuiTestEngineResultSummary summary;
    ImGuiTestEngine_GetResultSummary(engine, &summary);

    // IMPORTANT: we need to destroy the Dear ImGui context BEFORE the test
    //  engine context, so .ini data may be saved.
    ImGuiTestEngine_DestroyContext(engine);

    // Exit with error if not all tests passed
    if (summary.CountTested != summary.CountSuccess)
    {
       exit(EXIT_FAILURE); // Error
    }
    //exit(EXIT_SUCCESS); // OK
  #endif
  }

  void Application::quit()
  {
    running_ = false;
  }

} // namespace support
