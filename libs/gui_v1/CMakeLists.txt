
# GUI.v1 library

add_library(imgui_wrap_gui_v1 STATIC)

target_link_libraries(imgui_wrap_gui_v1 PUBLIC
  imgui imgui_zoomable_image std_compat imgui_wrap_common imgui_wrap_backends
)

target_include_directories(imgui_wrap_gui_v1
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_sources(imgui_wrap_gui_v1 PRIVATE
    # public api
    src/Widget.cpp
    src/Application.cpp
    src/Window.cpp
    src/Frame.cpp
    src/ChildFrame.cpp
    src/Sizer.cpp
    src/StackingSizer.cpp
    src/VerticalSizer.cpp
    src/HorizontalSizer.cpp
    src/Image.cpp
    src/imgui_stdlib.cpp

    # implementation
    src/impl/util.cpp
)

if (USE_GLFW_GL3)
  target_link_libraries(imgui_wrap_gui_v1 PUBLIC imgui_wrap_gl)
endif(USE_GLFW_GL3)

if (USE_IMPLOT)
  target_link_libraries(imgui_wrap_gui_v1 PUBLIC implot)
endif(USE_IMPLOT)

if (USE_GUI_TEST_ENGINE)
  target_link_libraries(imgui_wrap_gui_v1 PUBLIC imgui_wrap_testing)
endif(USE_GUI_TEST_ENGINE)

if (BUILD_GUI_EXAMPLES)
  add_subdirectory(examples)
endif(BUILD_GUI_EXAMPLES)
