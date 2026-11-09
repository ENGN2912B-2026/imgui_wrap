
#pragma once

#include <array>

// Enable math operators for ImVec2 and ImVec4
#define IMGUI_DEFINE_MATH_OPERATORS

#define IM_VEC2_CLASS_EXTRA \
    ImVec2(std::array<float,2> a) { x = a[0]; y = a[1]; }

#define IM_VEC4_CLASS_EXTRA \
    ImVec4(std::array<float,4> a) { x = a[0]; y = a[1]; z = a[2]; w = a[3]; }

// Disable legacy features / enforce using latest
#ifndef IMGUI_HAS_IMSTR
#define IMGUI_DISABLE_OBSOLETE_FUNCTIONS
#endif

#ifdef USE_GUI_TEST_ENGINE
    // Enable coroutine implementation using std::thread
    #define IMGUI_TEST_ENGINE_ENABLE_COROUTINE_STDTHREAD_IMPL 1

    // Bind Main Assert macro to we can easily recover/skip over an assert
    #ifndef IM_ASSERT
    #define IM_ASSERT(_EXPR) IM_TEST_ENGINE_ASSERT(_EXPR)
    // V_ASSERT_CONTRACT, assertMacro:IM_ASSERT
    #endif

    // Remaining template
    // Use relative path as this file may be compiled with different settings
    // Include base config
    #include "imgui_test_engine/imgui_test_engine/imgui_te_imconfig.h"
#endif
