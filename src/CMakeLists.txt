
add_library(imgui_wrap STATIC)

target_include_directories(imgui_wrap
  PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

add_subdirectory(math)
add_subdirectory(gl)
add_subdirectory(gui)
add_subdirectory(timer)
add_subdirectory(color)
