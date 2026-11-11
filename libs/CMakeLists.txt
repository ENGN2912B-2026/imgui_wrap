
add_subdirectory(std_compat)
add_subdirectory(common)
add_subdirectory(backends)

if (USE_GUI_TEST_ENGINE)
  add_subdirectory(testing)
endif()

if (USE_GLFW_GL3 OR USE_GLAD)
  add_subdirectory(gl)
endif()

if (USE_NFD_CPP)
  add_subdirectory(nfd)
endif()

if (USE_GUI_V1)
  add_subdirectory(gui_v1)
endif()

if (USE_GUI_V2)
  add_subdirectory(gui_v2)
endif()
