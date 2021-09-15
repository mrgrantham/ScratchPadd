cmake_minimum_required(VERSION 3.20)

project(scratchpadd)
set(CMAKE_CXX_STANDARD 20)

message("Language Version: C++${CMAKE_CXX_STANDARD}")


include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()
message(${CONAN_LIBS})

file(GLOB IMGUI_BINDINGS
        ${CONAN_SRC_DIRS_IMGUI}/imgui_impl_glfw.cpp
        ${CONAN_SRC_DIRS_IMGUI}/imgui_impl_opengl3.cpp)

add_executable(${PROJECT_NAME} src/main.cpp
                          ${IMGUI_BINDINGS})

if (APPLE)
  message("\nAPPLE BUILD: Silencing Deprecated OpenGL message. We don't care\n")
  target_compile_definitions(${PROJECT_NAME} PUBLIC GL_SILENCE_DEPRECATION)
endif ()

target_compile_options(${PROJECT_NAME} PUBLIC -Werror=inconsistent-missing-override)

target_include_directories(${PROJECT_NAME} 
        PUBLIC include
        PUBLIC ${CONAN_SRC_DIRS_IMGUI})

target_link_libraries(${PROJECT_NAME} ${CONAN_LIBS})