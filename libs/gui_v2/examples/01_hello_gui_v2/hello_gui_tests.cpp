//  Copyright (c) 2024-2026 Daniel Moreno. All rights reserved.
//

#include <testing/TestManager.hpp>
#include <gui2/Application.hpp>
#include <gui2/Runtime.hpp>

#include <imgui.h>
#include <imgui_stdlib.h>
#include <imgui_te_context.h>

#include <iostream>

// Declare the application instance makes `getApp()` available to the test code
DECLARE_APPLICATION(gui2::Application) // Test if this compiles correctly

// Register the tests with TestManager instance
REGISTER_TEST(registerHelloGuiTests)

// Test registration function
void registerHelloGuiTests(testing::TestEngine& engine)
{
  auto* e = engine.getEngine();
  ImGuiTest* t = NULL;

  // Test 1:
  // ------
  // Verifies that the GUI renders "Hello World!" text correctly.
  t = IM_REGISTER_TEST(e, "hello_gui_v2", "output_text");
  struct TestVars
  {
    bool guiRendered = false;
    std::string text;
  };
  t->SetVarsDataType<TestVars>();
  t->GuiFunc = [](ImGuiTestContext* ctx) // GuiFunc is called every frame to render the GUI
  {
    IM_UNUSED(ctx);
    auto& window = getApp().getWindow();
    auto& rt = gui2::Runtime::getInstance();

    TestVars& vars = ctx->GetVars<TestVars>();
    vars.guiRendered = true;

    ImGui::Begin("Test Window", NULL, ImGuiWindowFlags_NoSavedSettings);
    ImGui::LogToBuffer();
    window.displayContent(rt);
    vars.text = ctx->UiContext->LogBuffer.c_str();
    ImGui::LogFinish();
    ImGui::End();
  };
  t->TestFunc = [](ImGuiTestContext* ctx) // TestFunc is called after GuiFunc to perform the test assertions
  {
    TestVars& vars = ctx->GetVars<TestVars>();
    if (!vars.guiRendered)
    { //skip test if gui was not rendered
      return;
    }
    ctx->SetRef("Test Window");
    std::cout << "[TestFunc hello_gui_v2::output_text] vars.text = "
      << vars.text << std::endl;
    IM_CHECK_STR_EQ(vars.text.c_str(), "Hello World!");
  };

  // Test 2:
  // ------
  // Example of a test that modifies the GUI state and checks the result. This
  // test will create a text input field, modify its value, and verify that the
  // change is reflected in the GUI.
  t = IM_REGISTER_TEST(e, "hello_gui_v2", "text_input");
  t->SetVarsDataType<TestVars>();
  t->GuiFunc = [](ImGuiTestContext* ctx)
  {
    TestVars& vars = ctx->GetVars<TestVars>();
    ImGui::Begin("Test Window", NULL, ImGuiWindowFlags_NoSavedSettings);
    ImGui::InputText("##input_text", &vars.text);
    ImGui::End();
  };
  t->TestFunc = [](ImGuiTestContext* ctx)
  {
    TestVars& vars = ctx->GetVars<TestVars>();
    ctx->SetRef("Test Window");
    vars.text = "other text";
    ctx->ItemInputValue("##input_text", "abc");
    IM_CHECK_EQ(vars.text, "abc");
  };
}
