
set(SOURCES hello_gui.cpp)
set(LIBS imgui_wrap_gui_v2)

if (USE_GUI_TEST_ENGINE)
  set(SOURCES ${SOURCES} hello_gui_tests.cpp)
  set(LIBS ${LIBS} imgui_test_engine)
endif()

add_gui_example(NAME hello_gui_v2
  LIBS ${LIBS}
  SOURCES ${SOURCES})
