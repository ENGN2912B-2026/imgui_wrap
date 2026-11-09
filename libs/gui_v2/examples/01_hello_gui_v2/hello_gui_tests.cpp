//  Copyright (c) 2024-2026 Daniel Moreno. All rights reserved.
//

#include <testing/TestManager.hpp>
#include <gui2/Application.hpp>
#include <gui2/Runtime.hpp>
#include <gui2/Menu.hpp>
#include <gui2/Separator.hpp>
#include <gui2/UserCode.hpp>

#include <imgui.h>
#include <imgui_internal.h>
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

  t = IM_REGISTER_TEST(e, "hello_gui_v2", "menu_bar");
  struct MenuVars
  {
    int clicks = 0;
    bool checked = false;
    float expectedContentY = 0;
    float actualContentY = 0;
  };
  t->SetVarsDataType<MenuVars>();
  t->GuiFunc = [](ImGuiTestContext* ctx)
  {
    auto& vars = ctx->GetVars<MenuVars>();
    auto& rt = gui2::Runtime::getInstance();
    gui2::Window window;
    window.setMenuBar(gui2::MenuBar{
      gui2::Menu{"File",
        gui2::MenuItem{"Open", [&vars] { ++vars.clicks; }},
        gui2::Separator{},
        gui2::Menu{"More",
          gui2::MenuItem{"Nested", [&vars] { vars.clicks += 10; }}
        }
      },
      gui2::Menu{"Help", gui2::MenuItem{"About", &vars.checked}}
    });
    window.setContent(gui2::UserCode{[&vars] {
      vars.actualContentY = ImGui::GetCursorScreenPos().y;
      ImGui::TextUnformatted("Content");
    }});

    ImGui::Begin("Menu Test Window", nullptr,
                 ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_MenuBar);
    vars.expectedContentY = ImGui::GetCursorScreenPos().y;
    window.displayContent(rt);
    ImGui::End();
  };
  t->TestFunc = [](ImGuiTestContext* ctx)
  {
    auto& vars = ctx->GetVars<MenuVars>();
    ctx->SetRef("Menu Test Window");
    IM_CHECK_EQ(vars.actualContentY, vars.expectedContentY);
    ctx->MenuClick("File/Open");
    IM_CHECK_EQ(vars.clicks, 1);
    ctx->MenuClick("File/More/Nested");
    IM_CHECK_EQ(vars.clicks, 11);
    ctx->MenuClick("Help/About");
    IM_CHECK(vars.checked);
    ctx->MenuClick("Help/About");
    IM_CHECK(!vars.checked);
  };

  t = IM_REGISTER_TEST(e, "hello_gui_v2", "window_menu_bar");
  t->TestFunc = [](ImGuiTestContext* ctx)
  {
    auto& window = getApp().getWindow();
    window.setMenuBar(gui2::MenuBar{
      gui2::Menu{"File", gui2::MenuItem{"Remove bar", [&window] {
        window.clearMenuBar();
      }}}
    });
    ctx->Yield(2);
    IM_CHECK(ImGui::FindWindowByName("mainFrame")->Flags & ImGuiWindowFlags_MenuBar);
    ctx->SetRef("mainFrame");
    ctx->MenuClick("File/Remove bar");
    ctx->Yield(2);
    IM_CHECK(!window.hasMenuBar());
    IM_CHECK(!(ImGui::FindWindowByName("mainFrame")->Flags & ImGuiWindowFlags_MenuBar));
  };
}
