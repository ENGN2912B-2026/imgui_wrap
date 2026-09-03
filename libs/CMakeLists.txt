
add_subdirectory(std_compat)
add_subdirectory(common)
add_subdirectory(backends)

if (USE_GUI_TEST_ENGINE)
  add_subdirectory(testing)
endif(USE_GUI_TEST_ENGINE)

if (USE_GLFW_GL3)
  add_subdirectory(gl)
endif(USE_GLFW_GL3)

if (USE_GUI_V1)
  add_subdirectory(gui_v1)
endif(USE_GUI_V1)

if (USE_GUI_V2)
  add_subdirectory(gui_v2)
endif(USE_GUI_V2)
