//  Copyright (c) 2024-2026 Daniel Moreno. All rights reserved.
//

#include <gui/gui.hpp>
#include <testing/TestManager.hpp>

#include <imgui_te_context.h>

#include <iostream>

DECLARE_APPLICATION(gui::Application) // Test if this compiles correctly

REGISTER_TEST(registerHelloGuiTests)
void registerHelloGuiTests(testing::TestEngine& engine)
{
  auto* e = engine.getEngine();
  ImGuiTest* t = NULL;

  t = IM_REGISTER_TEST(e, "hello_gui", "output_text");
  struct TestVars
  {
    bool guiRendered = false;
    std::string text;
  };
  t->SetVarsDataType<TestVars>();
  t->GuiFunc = [](ImGuiTestContext* ctx)
  {
    IM_UNUSED(ctx);
    auto& frames = getApp().getWindow().getFrames();
    IM_CHECK_GE(frames.size(), 1);
    auto framePtr = frames.front();
    IM_CHECK(framePtr);

    TestVars& vars = ctx->GetVars<TestVars>();
    vars.guiRendered = true;

    ImGui::Begin("Test Window", NULL, ImGuiWindowFlags_NoSavedSettings);
    ImGui::LogToBuffer();
    framePtr->render();
    vars.text = ctx->UiContext->LogBuffer.c_str();
    ImGui::LogFinish();
    ImGui::End();
  };
  t->TestFunc = [](ImGuiTestContext* ctx)
  {
    TestVars& vars = ctx->GetVars<TestVars>();
    if (!vars.guiRendered)
    { //skip test if gui was not rendered
      return;
    }
    ctx->SetRef("Test Window");
    std::cout << "[TestFunc hello_gui::output_text] vars.text = "
      << vars.text << std::endl;
    IM_CHECK_STR_EQ(vars.text.c_str(), "Hello World!");
  };

  t = IM_REGISTER_TEST(e, "hello_gui", "text_input");
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
