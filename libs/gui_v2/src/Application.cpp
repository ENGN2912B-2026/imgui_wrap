//  Copyright (c) 2026 Daniel Moreno. All rights reserved.
//

#include <gui2/Application.hpp>
#include <gui2/Runtime.hpp>

#ifdef USE_GUI_TEST_ENGINE
# include <testing/TestEngine.hpp>
# include <testing/TestReporter.hpp>
# include <backend/Backend_Null.hpp>
//# include <imgui_te_engine.h>
# ifdef SHOW_TEST_ENGINE_WINDOWS
#  include <imgui_te_ui.h>
# endif
# include <iostream>
# include <fstream>
#endif

#include <stdexcept>

namespace gui2
{
  // static member initialization
  Application* Application::instance_ = nullptr;

  Application::Application(
    const std::string& windowTitle,
    const Vec2i& windowSize)
  {
    // Check if an instance already exists
    if (instance_)
    {
      throw std::logic_error{
        "Application instance already exists. Only one instance is allowed."};
    }
    // Initialize the application
    window_ = Window{windowTitle, windowSize};
    running_ = false;
    instance_ = this;
  }

  Application::~Application()
  {
    if (instance_ == this)
    {
      instance_ = nullptr;
    }
  }

  void Application::run()
  {
    auto& rt = Runtime::getInstance();

    // Initialize the runtime and create a desktop window
    rt.initialize(window_.getTitle(), window_.getSize());

#ifdef USE_GUI_TEST_ENGINE
    testing::TestEngine engine;
    engine.initialize();
    engine.enqueueTests();
#endif

    // Event loop
    running_ = true;
    std::string lastTitle = window_.getTitle();
    Vec2i lastSize = window_.getSize();
    while (running_ && rt.frameBegin())
    {
      //TODO: sync the window size with the runtime window size in a
      // thread -safe, in case the user resized the window.

      // Synchronize the window size three-way between the application window,
      // the runtime window, and the backend window.
      if (lastSize != window_.getSize())
      { // User has set a new size for the application window, so we need to
        // update the runtime window size.
        lastSize = window_.getSize();
        rt.setWindowSize(lastSize);
      }
      else
      { // Check if the runtime window size has changed (e.g., due to user
        // resizing the backend window)
        const Vec2i runtimeSize = rt.getWindowSize();
        if (lastSize != runtimeSize)
        {
          lastSize = runtimeSize;
          window_.setSize(lastSize);
        }
      }

      if (lastTitle != window_.getTitle())
      {
        lastTitle = window_.getTitle();
        rt.setWindowTitle(lastTitle);
      }

      window_.displayContent(rt);

#if defined(USE_GUI_TEST_ENGINE) && defined(SHOW_TEST_ENGINE_WINDOWS)
      ImGuiTestEngine_ShowTestEngineWindows(engine.getEngine(), NULL);
#endif

      rt.frameEnd();

#ifdef USE_GUI_TEST_ENGINE
      // Call after your rendering. This is mostly to support screen/video
      //  capturing features.
      engine.postRenderingUpdate();

      // Exit after running all tests if backend is null,
      // or if GUI_EXIT_AFTER_TESTING is set to 1
      const bool GUI_EXIT_AFTER_TESTING{
        std::getenv("GUI_EXIT_AFTER_TESTING") != nullptr
        && std::string{ std::getenv("GUI_EXIT_AFTER_TESTING") } == "1" };
      const bool USE_NULL_BACKEND{
        dynamic_cast<backend::Backend_Null*>(&rt.getBackend()) != nullptr };
      if (engine.testQueueEmpty() && (GUI_EXIT_AFTER_TESTING || USE_NULL_BACKEND))
      {
        break;
      }
#endif
    }

#ifdef USE_GUI_TEST_ENGINE
    engine.stop();
#endif

    // Uninitialize the runtime
    rt.uninitialize();

#ifdef USE_GUI_TEST_ENGINE
    { // Print test results
      auto reporter = testing::TestReporter::makeDefault();
      reporter->generateReport(engine, std::cout);
    }

    { // Create a markdown report
      const char* filename{ std::getenv("GUI_TEST_MARKDOWN_REPORT_FILE") };
      if (filename)
      {
        std::ofstream os{ filename };
        auto reporter = std::make_unique<testing::TestReporterMarkdown>();
        reporter->generateReport(engine, os);
      }
    }

    // Get test results
    const auto summary = engine.getResultSummary();

    // IMPORTANT: we need to destroy the Dear ImGui context BEFORE the test
    //  engine context, so .ini data may be saved.
    engine.uninitialize();

    // Exit with error if not all tests passed
    if (summary.CountTested != summary.CountSuccess)
    {
       exit(EXIT_FAILURE); // Error
    }
    //exit(EXIT_SUCCESS); // OK
#endif
  }

  Application& Application::getInstance()
  {
    if (!instance_)
    {
      throw std::logic_error{
        "Application instance does not exist. Create an instance first."};
    }
    return *instance_;
  }
}
