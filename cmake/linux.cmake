message(STATUS "Building for Linux")

add_executable(${PROJECT_NAME} 
                        ${SRC_FILES}
                        ${IMGUI_BINDINGS})