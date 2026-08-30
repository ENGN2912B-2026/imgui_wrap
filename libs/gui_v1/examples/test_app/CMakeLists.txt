
if (NOT USE_GUI_TEST_ENGINE)
  message("GUI test engine is not enabled, skipping test_app")
  return()
endif()

add_executable(test_app
  ${IMGUI_TEST_ENGINE_DIR}/app_minimal/app_minimal_main.cpp
  ${IMGUI_TEST_ENGINE_DIR}/app_minimal/app_minimal_tests.cpp
  ${IMGUI_TEST_ENGINE_DIR}/shared/imgui_app.cpp
  ${IMGUI_DIR}/imgui_demo.cpp
)

target_link_libraries(test_app PRIVATE imgui_test_engine)

target_include_directories(test_app
  PRIVATE
    ${IMGUI_TEST_ENGINE_DIR}
)

if(USE_GLFW_GL3)
  message("Using GLFW_GL3")
  target_compile_definitions(test_app PRIVATE -DIMGUI_APP_GLFW_GL3)
else()
  if(WIN32)
    message("Using WIN32_DX11")
    target_link_libraries(test_app
      PRIVATE d3d11.lib d3dcompiler.lib dxgi.lib shell32.lib)
    target_compile_definitions(test_app PRIVATE -DIMGUI_APP_WIN32_DX11)
  endif()
endif()