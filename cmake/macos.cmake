message(STATUS "Building for MacOS")

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

if (APPLE AND GRAPHICS_BACKEND STREQUAL "GL")
  message(STATUS "APPLE OPENGL BUILD: Silencing Deprecated OpenGL message. We don't care\n")
  target_compile_definitions(${PROJECT_NAME} PUBLIC GL_SILENCE_DEPRECATION)
endif ()