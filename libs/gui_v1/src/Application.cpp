//  Copyright (c) 2024-2026 Daniel Moreno. All rights reserved.
//

#include <gui/Application.hpp>

#ifdef USE_GUI_TEST_ENGINE
# include <testing/TestEngine.hpp>
# include <testing/TestReporter.hpp>
# include <backend/Backend_Null.hpp>
//# include <imgui_te_engine.h>
# ifdef SHOW_TEST_ENGINE_WINDOWS
#   include <imgui_te_ui.h>
# endif
# include <iostream>
# include <fstream>
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
    testing::TestEngine engine;
    engine.initialize();
    engine.enqueueTests();
#endif

    // main loop
    running_ = true;
    while (running_ && window_->renderBegin())
    {
      window_->render();
#if defined(USE_GUI_TEST_ENGINE) && defined(SHOW_TEST_ENGINE_WINDOWS)
      ImGuiTestEngine_ShowTestEngineWindows(engine.getEngine(), NULL);
#endif
      window_->renderEnd();

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
        dynamic_cast<backend::Backend_Null*>(window_->getBackendPtr()) != nullptr };
      if (engine.testQueueEmpty() && (GUI_EXIT_AFTER_TESTING || USE_NULL_BACKEND))
      {
        break;
      }
#endif
    }

#ifdef USE_GUI_TEST_ENGINE
    engine.stop();
#endif

    //deinit window
    window_->deinit();

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

  void Application::quit()
  {
    running_ = false;
  }

} // namespace support
