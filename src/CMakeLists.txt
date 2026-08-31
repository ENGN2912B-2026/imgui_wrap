
add_library(imgui_wrap STATIC)

target_include_directories(imgui_wrap
  PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(imgui_wrap PUBLIC imgui imgui_zoomable_image std_compat)

if (USE_IMPLOT)
  target_link_libraries(imgui_wrap PUBLIC implot)
  target_compile_definitions(imgui_wrap PUBLIC USE_IMPLOT)
endif(USE_IMPLOT)

if (USE_GUI_TEST_ENGINE)
  target_link_libraries(imgui_wrap PUBLIC imgui_test_engine)

  option(SHOW_TEST_ENGINE_WINDOWS "Show test engine windows" OFF)
  if (SHOW_TEST_ENGINE_WINDOWS)
    target_compile_definitions(imgui_wrap PRIVATE SHOW_TEST_ENGINE_WINDOWS)
  endif(SHOW_TEST_ENGINE_WINDOWS)

endif(USE_GUI_TEST_ENGINE)

# if (USE_NATIVEFILEDIALOG_EXTENDED)
#   target_link_libraries(imgui_wrap PUBLIC nfd)
#   target_compile_definitions(imgui_wrap PUBLIC USE_NFD)
# endif(USE_NATIVEFILEDIALOG_EXTENDED)

if (USE_ROBOTO_WEBFONT)
  target_link_libraries(imgui_wrap PUBLIC file_embed)
  target_compile_definitions(imgui_wrap PUBLIC USE_ROBOTO_WEBFONT)
endif(USE_ROBOTO_WEBFONT)


add_subdirectory(math)
add_subdirectory(gl)
add_subdirectory(gui)
add_subdirectory(timer)
add_subdirectory(color)

add_subdirectory(gui2)
