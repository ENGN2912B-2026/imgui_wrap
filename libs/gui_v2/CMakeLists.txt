
# GUI.v2 library

add_library(imgui_wrap_gui_v2 STATIC)

target_link_libraries(imgui_wrap_gui_v2 PUBLIC
  imgui imgui_zoomable_image std_compat imgui_wrap_common imgui_wrap_backends
)

target_include_directories(imgui_wrap_gui_v2
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_sources(imgui_wrap_gui_v2 PRIVATE
  src/Runtime.cpp
  src/Application.cpp
  src/Window.cpp
)

if (USE_GLFW_GL3)
  target_link_libraries(imgui_wrap_gui_v2 PUBLIC imgui_wrap_gl)
endif(USE_GLFW_GL3)

if (USE_IMPLOT)
  target_link_libraries(imgui_wrap_gui_v2 PUBLIC implot)
endif(USE_IMPLOT)

if (USE_GUI_TEST_ENGINE)
  target_link_libraries(imgui_wrap_gui_v2 PUBLIC imgui_wrap_testing)
endif(USE_GUI_TEST_ENGINE)

if (BUILD_GUI_EXAMPLES)
  add_subdirectory(examples)
endif(BUILD_GUI_EXAMPLES)
