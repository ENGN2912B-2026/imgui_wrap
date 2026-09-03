//  Copyright (c) 2026 Daniel Moreno. All rights reserved.
//

#pragma once

#include <imgui_te_engine.h>

#include <memory>

namespace testing
{
  //! \brief A class that encapsulates the ImGuiTestEngine context and provides
  //!        an interface for managing tests.
  class TestEngine
  {
  public:
    //! \brief Constructor.
    TestEngine() = default;

    //! \brief Delete copy constructor.
    TestEngine(const TestEngine&) = delete;

    //! \brief Move constructor.
    TestEngine(TestEngine&&) noexcept;

    //! \brief Destructor.
    ~TestEngine();

    //! \brief Delete copy assignment operator.
    TestEngine& operator=(const TestEngine&) = delete;

    //! \brief Move assignment operator.
    TestEngine& operator=(TestEngine&&) noexcept;

    //! \brief Check if the test engine is initialized.
    //! \return True if the test engine is initialized, false otherwise.
    bool isInitialized() const { return engine_ != nullptr; }

    //! \brief Initialize the test engine.
    void initialize();

    //! \brief Uninitialize the test engine.
    void uninitialize();

    //! \brief Enqueue all registered tests for execution.
    //! \throw std::runtime_error if the test engine is not initialized.
    void enqueueTests();

    //! \brief Check if the test queue is empty.
    //! \return True if the test queue is empty, false otherwise.
    //! \throw std::runtime_error if the test engine is not initialized.
    bool testQueueEmpty() const;

    //! \brief Update the test engine after rendering.
    //! \throw std::runtime_error if the test engine is not initialized.
    void postRenderingUpdate();

    //! \brief Stop the test engine and abort any running tests.
    //! \throw std::runtime_error if the test engine is not initialized.
    void stop();

    //! \brief Get the result summary of the test engine.
    //! \return An ImGuiTestEngineResultSummary structure containing the test
    //!         results.
    //! \throw std::runtime_error if the test engine is not initialized.
    //!
    //! The summary includes:
    //! - Total number of tests executed (CountTested)
    //! - Number of tests succeeded (CountSuccess)
    //! - Number of tests remaining in queue (CountInQueue)
    ImGuiTestEngineResultSummary getResultSummary() const;

    //! \brief Get the test engine context.
    //! \return The test engine context, or nullptr if not initialized.
    ImGuiTestEngine* getEngine() { return engine_; }

  private:
    ImGuiTestEngine* engine_ = nullptr;
  };

} // namespace testing
