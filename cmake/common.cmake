
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# CMP0077 NEW means that option() honors normal variable values, so we can set
# options in the command line or in the cache and they will be honored. This is
# useful for 3rd party libraries that use option() to control their build.
cmake_policy(SET CMP0077 NEW)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
  string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG}
      "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}")
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG}
      "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG}
      "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
endforeach(OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
  set(LINUX TRUE)
endif()

if(MSVC)
  add_compile_options(/permissive-)
endif()

if(APPLE)
  option(XCODE_UNIVERSAL_APP "Universal MacOS executable")
  if (XCODE_UNIVERSAL_APP)
    # Enable building of universal MacOS executable (works both on Intel
    # and ARM based Mac's). Requires Xcode 12.2 or newer.
    message(STATUS "Building MacOS universal executable")
    set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64" CACHE STRING "" FORCE)
  endif()
endif()

include(cmake/git_submodule.cmake)
include(cmake/gui_executable.cmake)
include(cmake/file_embed.cmake)
