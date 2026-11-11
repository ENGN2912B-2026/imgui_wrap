
# NFD (Native File Dialog) C++ wrapper

add_library(nfd_cpp STATIC)

target_link_libraries(nfd_cpp PUBLIC imgui_wrap_backends)

if (USE_NATIVEFILEDIALOG_EXTENDED)
  target_link_libraries(nfd_cpp PUBLIC nfd)
endif()

target_include_directories(nfd_cpp
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_sources(nfd_cpp PRIVATE
  src/OpenFileDialog.cpp
)

target_compile_definitions(nfd_cpp PUBLIC USE_NFD_CPP)
