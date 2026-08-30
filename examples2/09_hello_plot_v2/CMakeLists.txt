
if (USE_IMPLOT)
  add_gui_executable(hello_plot_v2 hello_plot.cpp)
  target_link_libraries(hello_plot_v2 PUBLIC imgui_wrap implot)
endif(USE_IMPLOT)
