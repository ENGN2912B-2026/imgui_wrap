
add_gui_example(NAME hello_file_v2
  LIBS imgui_wrap_gui_v2
  SOURCES hello_file.cpp)

if (TARGET hello_file_v2 AND USE_NATIVEFILEDIALOG_EXTENDED)
  target_link_libraries(hello_file_v2 PRIVATE nfd)
endif()

if (TARGET hello_file_v2 AND USE_NFD_CPP)
  target_link_libraries(hello_file_v2 PRIVATE nfd_cpp)
endif()
