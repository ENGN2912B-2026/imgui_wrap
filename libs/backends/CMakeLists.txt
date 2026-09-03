
# library with GUI backends

add_library(imgui_wrap_backends STATIC)

target_link_libraries(imgui_wrap_backends PUBLIC imgui imgui_wrap_common)

target_include_directories(imgui_wrap_backends
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_sources(imgui_wrap_backends PRIVATE
  src/Backend.cpp
  src/Backend_Null.cpp
)

if (USE_GLFW_GL3)
  target_sources(imgui_wrap_backends PRIVATE src/Backend_GLFW_GL3.cpp)
endif(USE_GLFW_GL3)

if (USE_GLAD)
  target_link_libraries(imgui_wrap_backends PUBLIC glad)
endif(USE_GLAD)

if (USE_ROBOTO_WEBFONT)
  target_link_libraries(imgui_wrap_backends PUBLIC file_embed)
  target_compile_definitions(imgui_wrap_backends PUBLIC USE_ROBOTO_WEBFONT)
endif(USE_ROBOTO_WEBFONT)

if (USE_GUI_TEST_ENGINE)
  target_link_libraries(imgui_wrap_backends PUBLIC imgui_test_engine)
  option(SHOW_TEST_ENGINE_WINDOWS "Show test engine windows" OFF)
  if (SHOW_TEST_ENGINE_WINDOWS)
    target_compile_definitions(imgui_wrap_backends PUBLIC SHOW_TEST_ENGINE_WINDOWS)
  endif(SHOW_TEST_ENGINE_WINDOWS)
endif(USE_GUI_TEST_ENGINE)
