
# modified from: https://gitlab.com/jhamberg/cmake-examples/-/blob/master/cmake/FileEmbed.cmake

# include this file only once
include_guard(GLOBAL)

# save the path to this file
set(FILE_EMBED_CMAKE_FILE_ "${CMAKE_CURRENT_LIST_FILE}")
mark_as_advanced(FILE_EMBED_CMAKE_FILE_)

# library holding embedded files
set(FILE_EMBED_LIB_ file_embed)
mark_as_advanced(FILE_EMBED_LIB_)

# directory where to generate files
set(FILE_EMBED_DIR_ ${CMAKE_BINARY_DIR}/${FILE_EMBED_LIB_})
mark_as_advanced(FILE_EMBED_DIR_)


function(FileEmbedSetup)

    if (NOT EXISTS ${FILE_EMBED_DIR_})
        file(MAKE_DIRECTORY ${FILE_EMBED_DIR_})
    endif ()

    set(place_holder_file ${FILE_EMBED_DIR_}/${FILE_EMBED_LIB_}_empty.cpp)

    if (NOT EXISTS ${place_holder_file})
        file(WRITE ${place_holder_file} "int _file_embed_version{ 1 };\n")
    endif ()

    add_library(${FILE_EMBED_LIB_} ${place_holder_file})
    target_include_directories(${FILE_EMBED_LIB_} PUBLIC ${FILE_EMBED_DIR_})

endfunction()

function(FileEmbedAdd file)

    get_filename_component(base_filename ${file} NAME)
    string(MAKE_C_IDENTIFIER ${base_filename} cpp_name)

    set(embed_dir ${FILE_EMBED_DIR_})
    set(generated_cpp ${embed_dir}/${cpp_name}.cpp)

    add_custom_command(
            OUTPUT ${generated_cpp}
            COMMAND ${CMAKE_COMMAND}
            -DRUN_FILE_EMBED_GENERATE=1
            -DFILE_EMBED_GENERATE_SOURCE=${file}
            -DFILE_EMBED_GENERATE_NAME=${cpp_name}
            -DFILE_EMBED_GENERATE_DIR=${embed_dir}
            -P ${FILE_EMBED_CMAKE_FILE_}
            MAIN_DEPENDENCY ${file}
    )

    target_sources(${FILE_EMBED_LIB_} PRIVATE ${generated_cpp})

endfunction()


function(FileEmbedGenerate file cpp_name embed_dir)

    file(READ ${file} content HEX)
    message("Embed file: ${file}")

    # Separate into individual bytes.
    string(REGEX MATCHALL "([A-Fa-f0-9][A-Fa-f0-9])" SEPARATED_HEX ${content})

    set(output_cpp "")

    set(counter 0)
    foreach (hex IN LISTS SEPARATED_HEX)
        string(APPEND output_cpp "0x${hex},")
        MATH(EXPR counter "${counter}+1")
        if (counter GREATER 16)
            string(APPEND output_cpp "\n    ")
            set(counter 0)
        endif ()
    endforeach ()

    set(output_cpp "
#include \"${cpp_name}.hpp\"
uint8_t ${cpp_name}_data[] = {
    ${output_cpp}
}\;
uint32_t ${cpp_name}_size = sizeof(${cpp_name}_data)\;
")

    set(output_hpp "
#pragma once
#include \"stdint.h\"
extern uint8_t ${cpp_name}_data[]\;
extern uint32_t ${cpp_name}_size\;
    ")


    if (NOT EXISTS ${embed_dir})
        file(MAKE_DIRECTORY ${embed_dir})
    endif ()

    file(WRITE ${embed_dir}/${cpp_name}.cpp ${output_cpp})
    message("Created: ${embed_dir}/${cpp_name}.cpp")
    file(WRITE ${embed_dir}/${cpp_name}.hpp ${output_hpp})
    message("Created: ${embed_dir}/${cpp_name}.hpp")

endfunction()

if (RUN_FILE_EMBED_GENERATE)
    message("---- RUN_FILE_EMBED_GENERATE")
    FileEmbedGenerate(${FILE_EMBED_GENERATE_SOURCE} ${FILE_EMBED_GENERATE_NAME} ${FILE_EMBED_GENERATE_DIR})
endif ()

if (NOT RUN_FILE_EMBED_GENERATE)
    # create the library holding the embedded files
    FileEmbedSetup()
endif()
