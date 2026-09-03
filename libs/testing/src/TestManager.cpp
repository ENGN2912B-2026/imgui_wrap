//  Copyright (c) 2026 Daniel Moreno. All rights reserved.
//

#include <testing/TestManager.hpp>
#include <testing/TestEngine.hpp>

namespace testing
{
  TestManager& TestManager::getInstance()
  {
    static TestManager instance;
    return instance;
  }

  void TestManager::registerTests(TestRegister& func)
  {
    registry_.emplace_back(func);
  }

  void TestManager::processRegistrations(TestEngine& engine)
  {
    for (auto& registerTest : registry_)
    {
      registerTest(engine);
    }
  }
}
