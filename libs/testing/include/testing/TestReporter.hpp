//  Copyright (c) 2024-2026 Daniel Moreno. All rights reserved.
//
#pragma once

#include <memory>
#include <ostream>

namespace testing
{
  // Forward declaration
  class TestEngine;

  //! \brief An abstract base class for test reporters.
  class TestReporter
  {
  public:
    //! \brief Virtual destructor.
    virtual ~TestReporter() = default;

    //! \brief Creates a default test reporter.
    //! \return A unique pointer to a TestReporter object.
    static std::unique_ptr<TestReporter> makeDefault();

    //! \brief Generates a report of the test results.
    //! \param[in] engine  The test engine containing the test results.
    //! \param[in] os      The output stream to write the report to.
    virtual void generateReport(TestEngine& engine, std::ostream& os) = 0;
  };

  //! \brief A test reporter that generates a report in console format.
  class TestReporterConsole : public TestReporter
  {
  public:
    //! \brief Generates a report of the test results in console format.
    //! \param[in] engine  The test engine containing the test results.
    //! \param[in] os      The output stream to write the report to.
    void generateReport(TestEngine& engine, std::ostream& os) override;
  };

  //! \brief A test reporter that generates a report in markdown format.
  class TestReporterMarkdown : public TestReporter
  {
  public:
    //! \brief Generates a report of the test results in markdown format.
    //! \param[in] engine  The test engine containing the test results.
    //! \param[in] os      The output stream to write the report to.
    void generateReport(TestEngine& engine, std::ostream& os) override;
  };

} // namespace testing
