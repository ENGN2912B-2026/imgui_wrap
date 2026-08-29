

if (USE_NATIVEFILEDIALOG_EXTENDED)
  add_gui_executable(hello_file_v2 hello_file.cpp)
  target_link_libraries(hello_file_v2 PUBLIC imgui_wrap nfd)
endif(USE_NATIVEFILEDIALOG_EXTENDED)
