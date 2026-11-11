
# NFD (Native File Dialog) C++ wrapper

if (NOT USE_NATIVEFILEDIALOG_EXTENDED)
  message(FATAL_ERROR "NFD C++ wrapper (nfd_cpp) requires USE_NATIVEFILEDIALOG_EXTENDED to be ON")
endif()

add_library(nfd_cpp STATIC)

target_link_libraries(nfd_cpp PUBLIC nfd imgui_wrap_backends)

target_include_directories(nfd_cpp
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_sources(nfd_cpp PRIVATE
  src/OpenFileDialog.cpp
)

target_compile_definitions(nfd_cpp PUBLIC USE_NFD_CPP)
