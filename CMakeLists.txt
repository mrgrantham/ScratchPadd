cmake_minimum_required(VERSION 3.13)

project(worker_system)
set(CMAKE_CXX_STANDARD 20)



include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()
message(${CONAN_LIBS})
add_executable(${PROJECT_NAME} src/main.cpp)

if (APPLE)
  message("\nAPPLE BUILD: Silencing Deprecated OpenGL message. We don't care\n")
  target_compile_definitions(${PROJECT_NAME} PUBLIC GL_SILENCE_DEPRECATION)
endif ()

include_directories(include)
target_link_libraries(${PROJECT_NAME} ${CONAN_LIBS})