
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

if (USE_GLFW_GL3)
  target_sources(imgui_wrap PRIVATE impl/Backend_GLFW_GL3.cpp)
endif(USE_GLFW_GL3)

if (USE_IMPLOT)
  #target_sources(imgui_wrap PRIVATE impl/ImPlot.cpp)
endif(USE_IMPLOT)

if (USE_GUI_TEST_ENGINE)
  target_sources(imgui_wrap PRIVATE impl/TestReporter.cpp)
endif(USE_GUI_TEST_ENGINE)
