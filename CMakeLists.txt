cmake_minimum_required(VERSION 3.31.6)

project(imgui_wrap)

# Detect if this is a standalone build (i.e. not included as a subdirectory
#  of another project)
if("${PROJECT_SOURCE_DIR}" STREQUAL "${CMAKE_SOURCE_DIR}")
  set(IMGUI_WRAP_STANDALONE_BUILD TRUE)
  message(STATUS "imgui_wrap stand-alone build: ${IMGUI_WRAP_STANDALONE_BUILD}")
else()
  set(IMGUI_WRAP_STANDALONE_BUILD FALSE)
endif()

include(cmake/build_config.cmake)

add_subdirectory(3rd-party)
add_subdirectory(libs)
