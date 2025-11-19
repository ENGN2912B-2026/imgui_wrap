
target_sources(imgui_wrap
  PRIVATE
    FrameBuffer.cpp
    Program.cpp
    Shader.cpp
    Shape.cpp
    Circle.cpp
    Sphere.cpp
)

if (USE_GLAD)
  target_link_libraries(imgui_wrap PUBLIC glad)
  target_compile_definitions(imgui_wrap PUBLIC USE_GLAD)
endif(USE_GLAD)
