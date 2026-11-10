//  Copyright (c) 2024-2026 Daniel Moreno. All rights reserved.
//

#include <testing/TestManager.hpp>
#include <gui2/Application.hpp>
#include <gui2/Runtime.hpp>
#include <gui2/Menu.hpp>
#include <gui2/Dialog.hpp>
#include <gui2/Layout.hpp>
#include <gui2/Button.hpp>
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

    ImGui::SetNextWindowPos({0, 0});
    ImGui::SetNextWindowSize({640, 480});
    ImGui::Begin("Menu Test Window", nullptr,
                 ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoSavedSettings |
                 ImGuiWindowFlags_MenuBar);
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

  t = IM_REGISTER_TEST(e, "hello_gui_v2", "window_content_rect");
  struct ContentVars
  {
    int width = 640;
    int height = 480;
    bool hasMenu = false;
    gui2::Rect expected;
    gui2::Rect actual;
  };
  t->SetVarsDataType<ContentVars>();
  t->GuiFunc = [](ImGuiTestContext* ctx)
  {
    struct ContentProbe
    {
      gui2::Rect* actual;
      gui2::Rect display(const gui2::Runtime&, const gui2::Rect& rect)
      {
        *actual = rect;
        return rect;
      }
    };

    auto& vars = ctx->GetVars<ContentVars>();
    gui2::Window window{"", {vars.width, vars.height}};
    if (vars.hasMenu)
    {
      window.setMenuBar(gui2::MenuBar{gui2::Menu{"File"}});
    }
    window.setContent(ContentProbe{&vars.actual});
    ImGui::SetNextWindowPos({0, 0});
    ImGui::SetNextWindowSize({static_cast<float>(vars.width),
                              static_cast<float>(vars.height)});
    const int flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoSavedSettings |
      (vars.hasMenu ? ImGuiWindowFlags_MenuBar : 0);
    ImGui::Begin("Content Rect Test", nullptr, flags);
    const ImVec2 origin = ImGui::GetCursorScreenPos();
    const ImVec2 size = ImGui::GetContentRegionAvail();
    vars.expected = {{static_cast<int>(origin.x), static_cast<int>(origin.y)},
                     {static_cast<int>(size.x), static_cast<int>(size.y)}};
    window.displayContent(gui2::Runtime::getInstance());
    ImGui::End();
  };
  t->TestFunc = [](ImGuiTestContext* ctx)
  {
    auto& vars = ctx->GetVars<ContentVars>();
    for (int i = 0; i < 4; ++i)
    {
      vars.hasMenu = i % 2 != 0;
      vars.width = i < 2 ? 640 : 500;
      vars.height = i < 2 ? 480 : 300;
      ctx->Yield(2);
      IM_CHECK_EQ(vars.actual.origin.x, vars.expected.origin.x);
      IM_CHECK_EQ(vars.actual.origin.y, vars.expected.origin.y);
      IM_CHECK_EQ(vars.actual.size.x, vars.expected.size.x);
      IM_CHECK_EQ(vars.actual.size.y, vars.expected.size.y);
    }
  };

  t = IM_REGISTER_TEST(e, "hello_gui_v2", "menu_bar_rect");
  struct MenuRectVars
  {
    gui2::Rect expected;
    gui2::Rect actual;
    float framePaddingY = 3.0f;
  };
  t->SetVarsDataType<MenuRectVars>();
  t->GuiFunc = [](ImGuiTestContext* ctx)
  {
    auto& vars = ctx->GetVars<MenuRectVars>();
    gui2::MenuBar menu{gui2::Menu{"File"}};
    gui2::MenuBar copied = menu;
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{4.0f, vars.framePaddingY});
    ImGui::SetNextWindowPos({0, 0});
    ImGui::SetNextWindowSize({640, 480});
    ImGui::Begin("Menu Rect Test", nullptr,
                 ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoSavedSettings |
                 ImGuiWindowFlags_MenuBar);
    const ImRect bounds = ImGui::GetCurrentWindow()->MenuBarRect();
    vars.expected = {{static_cast<int>(bounds.Min.x), static_cast<int>(bounds.Min.y)},
                     {static_cast<int>(bounds.GetWidth()), static_cast<int>(bounds.GetHeight())}};
    vars.actual = gui2::Runtime::getInstance().display(copied, {{0, 0}, {640, 480}});
    ImGui::End();
    ImGui::PopStyleVar();
  };
  t->TestFunc = [](ImGuiTestContext* ctx)
  {
    auto& vars = ctx->GetVars<MenuRectVars>();
    for (float padding : {3.0f, 9.0f})
    {
      vars.framePaddingY = padding;
      ctx->Yield(2);
      IM_CHECK_EQ(vars.actual.origin.x, vars.expected.origin.x);
      IM_CHECK_EQ(vars.actual.origin.y, vars.expected.origin.y);
      IM_CHECK_EQ(vars.actual.size.x, vars.expected.size.x);
      IM_CHECK_EQ(vars.actual.size.y, vars.expected.size.y);
      IM_CHECK(vars.actual.size.y > 0);
    }
  };

  t = IM_REGISTER_TEST(e, "hello_gui_v2", "modal_dialog");
  struct DialogVars
  {
    gui2::Dialog dialog{"Dialog Test"};
  };
  t->SetVarsDataType<DialogVars>();
  t->GuiFunc = [](ImGuiTestContext* ctx)
  {
    auto& vars = ctx->GetVars<DialogVars>();
    auto& dialog = vars.dialog;
    dialog.setSize({600, 170});
    dialog.setCentered(true);
    dialog.setResizable(false);
    dialog.setNoScrollbar(true);
    dialog.setContent(gui2::VStack{
      "About",
      gui2::Separator{},
      gui2::HBox{
        gui2::Stretch{1, gui2::Empty{}},
        gui2::Fixed{120, gui2::Button{"OK", [&dialog] {
          dialog.close();
        }}.setDefaultFocus(true)},
      },
    });

    gui2::Window window;
    const ImVec2 viewportSize = ImGui::GetMainViewport()->Size;
    window.setSize({static_cast<int>(viewportSize.x), static_cast<int>(viewportSize.y)});
    window.setMenuBar(gui2::MenuBar{
      gui2::Menu{"Help", gui2::MenuItem{"About", [&dialog] { dialog.open(); }}}
    });
    window.addDialog(dialog);
    window.setContent("Content");
    ImGui::SetNextWindowPos({0, 0});
    ImGui::SetNextWindowSize(viewportSize);
    ImGui::Begin("Dialog Parent", nullptr,
                 ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoSavedSettings |
                 ImGuiWindowFlags_MenuBar);
    window.displayContent(gui2::Runtime::getInstance());
    ImGui::End();
  };
  t->TestFunc = [](ImGuiTestContext* ctx)
  {
    gui2::Button button{"Test"};
    auto& configured = button.setColor(gui2::Vec4f{0.25f, 0.5f, 0.75f, 1.0f})
                             .setDefaultFocus(true);
    IM_CHECK(&configured == &button);
    IM_CHECK(button.getColor().has_value());
    IM_CHECK_EQ(button.getColor()->x, 0.25f);
    IM_CHECK(button.getDefaultFocus());
    button.setColor(std::nullopt);
    IM_CHECK(!button.getColor().has_value());

    ctx->SetRef("Dialog Parent");
    ctx->MenuClick("Help/About");
    ctx->Yield(2);
    ImGuiWindow* popup = ImGui::FindWindowByName("Dialog Test");
    IM_CHECK(popup != nullptr && popup->WasActive);
    if (popup)
    {
      IM_CHECK(popup->Flags & ImGuiWindowFlags_Modal);
      IM_CHECK(popup->Flags & ImGuiWindowFlags_NoResize);
      IM_CHECK(popup->Flags & ImGuiWindowFlags_NoScrollbar);
      IM_CHECK_EQ(popup->Size.x, 600.0f);
      IM_CHECK_EQ(popup->Size.y, 170.0f);
      const ImVec2 center = ImGui::GetMainViewport()->GetCenter();
      IM_CHECK_EQ(popup->Pos.x + popup->Size.x * 0.5f, center.x);
      IM_CHECK_EQ(popup->Pos.y + popup->Size.y * 0.5f,
                  center.y + ImGui::GetFrameHeight() * 0.5f);
      IM_CHECK_EQ(GImGui->NavId, popup->GetID("OK"));
    }
    ctx->SetRef("Dialog Test");
    const auto ok = ctx->ItemInfo("OK");
    IM_CHECK_EQ(ok.RectFull.GetWidth(), 120.0f);
    IM_CHECK_EQ(ok.RectFull.Max.x, popup->WorkRect.Max.x);
    ctx->ItemClick("OK");
    ctx->Yield(2);
    IM_CHECK(!popup->WasActive);

    ctx->SetRef("Dialog Parent");
    ctx->MenuClick("Help/About");
    ctx->Yield(2);
    IM_CHECK(popup->WasActive);
    ctx->GetVars<DialogVars>().dialog.close();
    ctx->Yield(3);
    IM_CHECK(!popup->WasActive);
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
