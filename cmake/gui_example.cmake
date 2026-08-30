
# include this file only once
include_guard(GLOBAL)

# Add an example executable if all required library targets exist.
#
# Usage:
#
#   add_gui_example(
#     NAME  <example-name>
#     LIBS  <library-target>...
#     SOURCES <source-file>...
#   )
#
# NAME
#   Name of the example executable.
#
# SOURCES
#   Source files used to build the example.
#
# LIBS
#   Library targets required by the example. The example is only created
#   if all specified targets exist.
#
# Example:
#
#   add_gui_example(
#     NAME    hello_shader_v2
#     LIBS    imgui_wrap
#     SOURCES hello_shader.cpp
#   )
#
function(add_gui_example)
  set(options)
  set(oneValueArgs NAME)
  set(multiValueArgs SOURCES LIBS)

  cmake_parse_arguments(
    ARG
    "${options}"
    "${oneValueArgs}"
    "${multiValueArgs}"
    ${ARGN}
  )

  # Check that all required library targets exist.
  foreach(lib IN LISTS ARG_LIBS)
    if(NOT TARGET ${lib})
      message(STATUS
        "EXAMPLE ${ARG_NAME}: disabled (missing target ${lib})"
      )
      return()
    endif()
  endforeach()

  message(STATUS "EXAMPLE ${ARG_NAME}: enabled")

  add_gui_executable(${ARG_NAME} ${ARG_SOURCES})
  target_link_libraries(${ARG_NAME} PUBLIC ${ARG_LIBS})

endfunction(add_gui_example)
