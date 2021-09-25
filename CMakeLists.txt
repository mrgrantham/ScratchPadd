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

add_bundle_resources(RESOURCES "${CMAKE_SOURCE_DIR}/include/ScratchPadd/Graphics/GL/Shaders" "Resources")
add_bundle_resources(RESOURCES "${CMAKE_SOURCE_DIR}/resources" "Resources")

add_executable(${PROJECT_NAME} 
                        MACOSX_BUNDLE
                        ${SRC_FILES}
                        ${IMGUI_BINDINGS}
                        ${RESOURCES})

set_target_properties(${PROJECT_NAME} PROPERTIES
        RESOURCE ${PROJECT_NAME}.icns
        MACOSX_BUNDLE_SHORT_VERSION_STRING "0.1"
        MACOSX_BUNDLE_LONG_VERSION_STRING "0.1.1"
        MACOSX_BUNDLE_ICON_FILE ${PROJECT_NAME}.icns
)

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

if (APPLE AND GRAPHICS_BACKEND STREQUAL "GL")
  message(STATUS "APPLE OPENGL BUILD: Silencing Deprecated OpenGL message. We don't care\n")
  target_compile_definitions(${PROJECT_NAME} PUBLIC GL_SILENCE_DEPRECATION)
endif ()

target_compile_definitions(${PROJECT_NAME} PUBLIC PROJECT_NAME="${PROJECT_NAME}")


target_compile_options(${PROJECT_NAME} PUBLIC -Werror=inconsistent-missing-override
                                                -Werror=delete-abstract-non-virtual-dtor)

target_include_directories(${PROJECT_NAME} 
        PUBLIC include
        PUBLIC ${CONAN_SRC_DIRS_IMGUI})

target_link_libraries(${PROJECT_NAME} ${CONAN_LIBS})