
# common library with functions and utilities for the project

add_library(imgui_wrap_common STATIC)

target_link_libraries(imgui_wrap_common PUBLIC std_compat)

target_include_directories(imgui_wrap_common
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_sources(imgui_wrap_common PRIVATE
  # timer
  src/timer/Timer.cpp
  # color
  src/color/Colormap_turbo.cpp
  src/color/Colormap_glasbey.cpp
)
