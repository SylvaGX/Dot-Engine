cmake_minimum_required(VERSION 3.16)

if(APPLE)
    project(ImGui LANGUAGES CXX OBJCXX)
else()
    project(ImGui LANGUAGES CXX)
endif()

# Reference the imgui submodule directory
set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../imgui)

# Apply patch to fix DC bug in TheCherno's imgui fork
set(IMGUI_CPP "${IMGUI_DIR}/imgui.cpp")
set(IMGUI_HASH_FILE "${CMAKE_CURRENT_SOURCE_DIR}/imgui_dc_fix.hash")

message(STATUS "Checking ImGui DC fix state...")

# If the hash file does not exist, we must run the fix
set(RUN_IMGUI_FIX FALSE)

if(NOT EXISTS "${IMGUI_HASH_FILE}")
    message(STATUS "ImGui DC hash not found — fix required")
    set(RUN_IMGUI_FIX TRUE)
else()
    # Read previous hash
    file(READ "${IMGUI_HASH_FILE}" PREVIOUS_IMGUI_HASH)

    # Empty hash file → must run
    if(PREVIOUS_IMGUI_HASH STREQUAL "")
        message(STATUS "ImGui DC hash file empty — fix required")
        set(RUN_IMGUI_FIX TRUE)
    else()
        # Compute current hash
        file(SHA256 "${IMGUI_CPP}" CURRENT_IMGUI_HASH)

        if(NOT CURRENT_IMGUI_HASH STREQUAL PREVIOUS_IMGUI_HASH)
            message(STATUS "ImGui changed — fix required")
            set(RUN_IMGUI_FIX TRUE)
        else()
            message(STATUS "ImGui unchanged — DC fix not needed")
        endif()
    endif()
endif()

if(RUN_IMGUI_FIX)
    message(STATUS "Applying ImGui DC fix...")

    if(WIN32)
        set(IMGUI_FIX_SCRIPT "${CMAKE_SOURCE_DIR}/scripts/setup-imgui.bat")

        if(NOT EXISTS "${IMGUI_FIX_SCRIPT}")
            message(FATAL_ERROR "ImGui fix script not found: ${IMGUI_FIX_SCRIPT}")
        endif()

        execute_process(
                COMMAND cmd /c "${IMGUI_FIX_SCRIPT}"
                WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
                RESULT_VARIABLE SCRIPT_RESULT
        )
    else()
        set(IMGUI_FIX_SCRIPT "${CMAKE_SOURCE_DIR}/scripts/setup-imgui.sh")

        if(NOT EXISTS "${IMGUI_FIX_SCRIPT}")
            message(FATAL_ERROR "ImGui fix script not found: ${IMGUI_FIX_SCRIPT}")
        endif()

        execute_process(
                COMMAND bash "${IMGUI_FIX_SCRIPT}"
                WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
                RESULT_VARIABLE SCRIPT_RESULT
        )
    endif()

    if(SCRIPT_RESULT EQUAL 0)
        # Hash AFTER the fix
        file(SHA256 "${IMGUI_CPP}" FIXED_IMGUI_HASH)
        file(WRITE "${IMGUI_HASH_FILE}" "${FIXED_IMGUI_HASH}")
        message(STATUS "ImGui DC fix applied successfully")
    else()
        message(FATAL_ERROR "Failed to apply ImGui DC fix")
    endif()
endif()

set(IMGUI_SOURCES
    ${IMGUI_DIR}/imconfig.h
    ${IMGUI_DIR}/imgui.h
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_internal.h
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/imstb_rectpack.h
    ${IMGUI_DIR}/imstb_textedit.h
    ${IMGUI_DIR}/imstb_truetype.h
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
)

if(APPLE)
    list(APPEND IMGUI_SOURCES
        ${IMGUI_DIR}/backends/imgui_impl_metal.mm
    )
else()
    list(APPEND IMGUI_SOURCES
        ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
    )
endif()

add_library(ImGui STATIC ${IMGUI_SOURCES})

target_include_directories(ImGui PUBLIC
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
)

target_compile_definitions(ImGui PUBLIC GLFW_INCLUDE_NONE)

target_link_libraries(ImGui PUBLIC glfw)

if(APPLE)
    target_compile_definitions(ImGui PUBLIC GLFW_EXPOSE_NATIVE_COCOA)

    if(NOT TARGET MetalCPP)
        message(FATAL_ERROR "MetalCPP target not found. Add DotEngine/vendor/metal-cpp before imgui_cmake on macOS.")
    endif()

    # Private: compile imgui_impl_metal.mm only; engine links MetalCPP directly
    target_link_libraries(ImGui PRIVATE MetalCPP)
elseif(WIN32)
    target_link_libraries(ImGui PUBLIC opengl32)
elseif(UNIX)
    find_package(OpenGL REQUIRED)
    target_link_libraries(ImGui PUBLIC OpenGL::GL)
    set_target_properties(ImGui PROPERTIES POSITION_INDEPENDENT_CODE ON)
endif()

# C++ standard
target_compile_features(ImGui PUBLIC cxx_std_17)
