//  Copyright (c) 2024-2026 Daniel Moreno. All rights reserved.
//

#pragma once

#include <testing/TestEngine.hpp>

#include <memory>
#include <string>
#include <functional>

namespace testing
{
  //! \brief A singleton class that manages the registration and execution of
  //!        tests.
  class TestManager
  {
  public:
    //! \brief Type alias for a test registration function.
    using TestRegister = void(TestEngine&);

    //! \brief Get the singleton instance of the TestManager.
    //! \return A reference to the singleton instance of the TestManager.
    static TestManager& getInstance();

    //! \brief Register a test registration function.
    //! \param[in] func  A function pointer to the test registration function.
    void registerTests(TestRegister& func);

    //! \brief Process all registered test registration functions.
    //! \param[in] engine  A pointer to the ImGuiTestEngine context.
    //!
    //! This method iterates through all registered test registration functions
    //! and invokes them, passing the provided ImGuiTestEngine context.
    void processRegistrations(TestEngine& engine);

  private:
    std::vector<TestRegister*> registry_;

    //! \brief Private constructor to enforce singleton pattern.
    TestManager() = default;

    //! \brief Copy constructor is deleted to prevent copying of the singleton
    //!        instance.
    TestManager(const TestManager&) = delete;

    //! \brief Copy assignment operator is deleted to prevent copying of the
    //!        singleton instance.
    TestManager& operator=(const TestManager&) = delete;
  };

} // namespace testing

#define REGISTER_TEST(testRegistrationFunction) \
  void testRegistrationFunction(testing::TestEngine& engine);  \
  namespace { \
    static bool testRegistrationFunction_{ ( \
      testing::TestManager::getInstance().registerTests(testRegistrationFunction), true) }; \
  }
