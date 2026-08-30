
# library with functionality for testing

add_library(imgui_wrap_testing STATIC)

target_link_libraries(imgui_wrap_testing PUBLIC std_compat imgui_test_engine)

target_include_directories(imgui_wrap_testing
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_sources(imgui_wrap_testing PRIVATE
  src/TestEngine.cpp
  src/TestManager.cpp
  src/TestReporter.cpp
)
