
include(cmake/common.cmake)

option(USE_GLFW_GL3 "Enable GLFW/OpenGL3 backend" ON)
option(USE_IMPLOT "Enable ImPlot" ON)
option(USE_GUI_TEST_ENGINE "Enable Dear ImGui test engine" OFF)
option(USE_NATIVEFILEDIALOG_EXTENDED "Enable NativeFileDialog Extended" ON)
option(USE_GLAD "Enable GLAD OpenGL loader-generator" ON)
option(USE_ROBOTO_WEBFONT "Enable Roboto webfont" ON)

option(USE_GUI_V1 "Enable Gui version 1" OFF)
option(USE_GUI_V2 "Enable Gui version 2" ON)
option(BUILD_GUI_EXAMPLES "Build GUI examples" ${IMGUI_WRAP_STANDALONE_BUILD})

message(STATUS "USE_GUI_V1: ${USE_GUI_V1}")
message(STATUS "USE_GUI_V2: ${USE_GUI_V2}")
message(STATUS "BUILD_GUI_EXAMPLES: ${BUILD_GUI_EXAMPLES}")

set(IMGUI_DIR ${PROJECT_SOURCE_DIR}/3rd-party/imgui)
checkout_submodules(${IMGUI_DIR})

set(IMGUI_ZOOMABLE_IMAGE_DIR ${PROJECT_SOURCE_DIR}/3rd-party/imgui_zoomable_image)
checkout_submodules(${IMGUI_ZOOMABLE_IMAGE_DIR})

if(USE_GLFW_GL3)
  set(GLFW_DIR ${PROJECT_SOURCE_DIR}/3rd-party/glfw)
  checkout_submodules(${GLFW_DIR})
endif()
if(USE_IMPLOT)
  set(IMPLOT_DIR ${PROJECT_SOURCE_DIR}/3rd-party/implot)
  checkout_submodules(${IMPLOT_DIR})
endif()
if(USE_GUI_TEST_ENGINE)
  set(IMGUI_TEST_ENGINE_DIR ${PROJECT_SOURCE_DIR}/3rd-party/imgui_test_engine)
  checkout_submodules(${IMGUI_TEST_ENGINE_DIR})
endif()
if (USE_NATIVEFILEDIALOG_EXTENDED)
  set(NFD_DIR ${PROJECT_SOURCE_DIR}/3rd-party/nativefiledialog-extended)
  checkout_submodules(${NFD_DIR})
endif()
if (USE_ROBOTO_WEBFONT)
  FileEmbedAdd(${CMAKE_CURRENT_LIST_DIR}/../fonts/roboto-regular-webfont.ttf)
endif(USE_ROBOTO_WEBFONT)


if (BUILD_GUI_EXAMPLES)
  include(cmake/gui_example.cmake)
endif(BUILD_GUI_EXAMPLES)
