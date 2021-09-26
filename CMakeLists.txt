cmake_minimum_required(VERSION 3.20)
include(cmake/helpers.cmake)

project(ScratchPadd)
set(CMAKE_CXX_STANDARD 20)

message("Language Version: C++${CMAKE_CXX_STANDARD}")

option(TESTS "Build Scratchpadd Test Binary" OFF)


include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()
# message(${CONAN_LIBS})

file(GLOB IMGUI_BINDINGS
        ${CONAN_SRC_DIRS_IMGUI}/imgui_impl_glfw.cpp
        ${CONAN_SRC_DIRS_IMGUI}/imgui_impl_opengl3.cpp)

file(GLOB SRC_FILES src/*.cpp)

if (APPLE)
        include(cmake/macos.cmake)
elseif(UNIX)
        include(cmake/linux.cmake)
endif()

if (NOT DEFINED GRAPHICS_BACKEND)
        target_compile_definitions(${PROJECT_NAME} PUBLIC SCRATCHPADD_GL)
else()
        target_compile_definitions(${PROJECT_NAME} PUBLIC SCRATCHPADD_${GRAPHICS_BACKEND})
endif()
message(STATUS "Graphics Backend: ${GRAPHICS_BACKEND}")

if (TESTS)
        target_compile_definitions(${PROJECT_NAME} PUBLIC SCRATCHPADD_TESTS)
endif()
message(STATUS "Tests: ${TESTS}")

target_compile_definitions(${PROJECT_NAME} PUBLIC PROJECT_NAME="${PROJECT_NAME}")


if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
target_compile_options(${PROJECT_NAME} PUBLIC -Werror=inconsistent-missing-override
                                                -Werror=delete-abstract-non-virtual-dtor)
endif()


target_include_directories(${PROJECT_NAME} 
        PUBLIC include
        PUBLIC ${CONAN_SRC_DIRS_IMGUI})

target_link_libraries(${PROJECT_NAME} ${CONAN_LIBS})