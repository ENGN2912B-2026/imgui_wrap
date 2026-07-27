
target_sources(imgui_wrap
  PRIVATE
    # public api
    Widget.cpp
    Application.cpp
    Window.cpp
    Frame.cpp
    ChildFrame.cpp
    Sizer.cpp
    StackingSizer.cpp
    VerticalSizer.cpp
    HorizontalSizer.cpp
    Image.cpp
    imgui_stdlib.cpp

    # implementation
    impl/Backend.cpp
    impl/Backend_Null.cpp
    impl/util.cpp
)

target_link_libraries(imgui_wrap PUBLIC imgui imgui_zoomable_image)

if (USE_GLFW_GL3)
  target_sources(imgui_wrap PRIVATE impl/Backend_GLFW_GL3.cpp)
endif(USE_GLFW_GL3)

if (USE_IMPLOT)
  #target_sources(imgui_wrap PRIVATE impl/ImPlot.cpp)
  target_link_libraries(imgui_wrap PUBLIC implot)
  target_compile_definitions(imgui_wrap PUBLIC USE_IMPLOT)
endif(USE_IMPLOT)

if (USE_GUI_TEST_ENGINE)
  target_sources(imgui_wrap PRIVATE impl/TestReporter.cpp)
  target_link_libraries(imgui_wrap PUBLIC imgui_test_engine)

  option(SHOW_TEST_ENGINE_WINDOWS "Show test engine windows" OFF)
  if (SHOW_TEST_ENGINE_WINDOWS)
    target_compile_definitions(imgui_wrap PRIVATE SHOW_TEST_ENGINE_WINDOWS)
  endif(SHOW_TEST_ENGINE_WINDOWS)

endif(USE_GUI_TEST_ENGINE)

if (USE_ROBOTO_WEBFONT)
  target_link_libraries(imgui_wrap PUBLIC file_embed)
  target_compile_definitions(imgui_wrap PUBLIC USE_ROBOTO_WEBFONT)
endif(USE_ROBOTO_WEBFONT)
