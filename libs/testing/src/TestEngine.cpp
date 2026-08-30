//  Copyright (c) 2026 Daniel Moreno. All rights reserved.
//

#include <testing/TestEngine.hpp>
#include <testing/TestManager.hpp>

#include <imgui_te_internal.h>

#include <stdexcept>

namespace testing
{
  TestEngine::TestEngine(TestEngine&& other) noexcept : TestEngine{}
  {
    operator=(std::move(other));
  }

  TestEngine& TestEngine::operator=(TestEngine&& other) noexcept
  {
    if (this != &other)
    {
      // Move out from `other`
      engine_ = std::move(other.engine_);

      // Reset the other test engine to a default state
      other.engine_ = nullptr;
    }
    return *this;
  }

  TestEngine::~TestEngine()
  {
    uninitialize();
  }

  void TestEngine::initialize()
  {
    if (isInitialized())
    { // Already initialized
      return;
    }

    // Create test engine context
    engine_ = ImGuiTestEngine_CreateContext();
    if (!engine_)
    {
      throw std::runtime_error("Failed to create ImGuiTestEngine context");
    }

    // Setup test engine
    ImGuiTestEngineIO& test_io = ImGuiTestEngine_GetIO(engine_);
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
    ImGuiTestEngine_Start(engine_, ImGui::GetCurrentContext());
    ImGuiTestEngine_InstallDefaultCrashHandler();

    // Register tests
    TestManager::getInstance().processRegistrations(*this);

    //ImGuiTestEngine_QueueTests(engine, ImGuiTestGroup_Tests, "all");
  }

  void TestEngine::uninitialize()
  {
    if (engine_)
    {
      // IMPORTANT: we need to destroy the Dear ImGui context BEFORE the test
      //  engine context, so .ini data may be saved.
      ImGuiTestEngine_DestroyContext(engine_);
      engine_ = nullptr;
    }
  }

  void TestEngine::enqueueTests()
  {
    if (!isInitialized())
    {
      throw std::runtime_error("Test engine is not initialized");
    }
    ImGuiTestEngine_QueueTests(engine_, ImGuiTestGroup_Tests, "all");
  }

  bool TestEngine::testQueueEmpty() const
  {
    if (!isInitialized())
    {
      throw std::runtime_error("Test engine is not initialized");
    }
    return ImGuiTestEngine_IsTestQueueEmpty(engine_);
  }

  void TestEngine::postRenderingUpdate()
  {
    if (!isInitialized())
    {
      throw std::runtime_error("Test engine is not initialized");
    }
    ImGuiTestEngine_PostSwap(engine_);
  }

  void TestEngine::stop()
  {
    if (!isInitialized())
    {
      throw std::runtime_error("Test engine is not initialized");
    }
    ImGuiTestEngine_Stop(engine_);
  }

  ImGuiTestEngineResultSummary TestEngine::getResultSummary() const
  {
    if (!isInitialized())
    {
      throw std::runtime_error("Test engine is not initialized");
    }
    ImGuiTestEngineResultSummary summary;
    ImGuiTestEngine_GetResultSummary(engine_, &summary);
    return summary;
  }
}