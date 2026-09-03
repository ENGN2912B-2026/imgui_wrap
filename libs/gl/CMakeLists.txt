
# library with OpenGL functionality

add_library(imgui_wrap_gl STATIC)

target_link_libraries(imgui_wrap_gl PUBLIC imgui_wrap_common)

target_include_directories(imgui_wrap_gl
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_sources(imgui_wrap_gl PRIVATE
  src/FrameBuffer.cpp
  src/RenderBuffer.cpp
  src/Texture.cpp
  src/Program.cpp
  src/Shader.cpp
  src/Shape.cpp
  src/Circle.cpp
  src/Sphere.cpp
)

if (USE_GLAD)
  target_link_libraries(imgui_wrap_gl PUBLIC glad)
endif(USE_GLAD)
