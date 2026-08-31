cmake_minimum_required(VERSION 3.31.6)

project(imgui_wrap)

include(cmake/build_config.cmake)

add_subdirectory(3rd-party)
add_subdirectory(libs/std_compat)
add_subdirectory(src)

if("${PROJECT_SOURCE_DIR}" STREQUAL "${CMAKE_SOURCE_DIR}")
  message(STATUS "Stand-alone build: enable examples")
  add_subdirectory(examples)
  add_subdirectory(examples2)
endif()
