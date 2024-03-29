function(find_sources_and_include_directories HEADERS_PATH SOURCES_PATH)
    include_recursive_dirs(${HEADERS_PATH})
    file(GLOB_RECURSE FOUND_SOURCES ${SOURCES_PATH})

    set(SOURCES
        ${SOURCES}
        ${FOUND_SOURCES}
        CACHE INTERNAL "sources" FORCE)
    list(REMOVE_DUPLICATES SOURCES)
endfunction(find_sources_and_include_directories)

function(include_recursive_dirs HEADERS_PATH)
    file(GLOB_RECURSE FOUND_HEADERS ${HEADERS_PATH})

    set(INCLUDE_DIRS "")
    foreach(_headerFile ${FOUND_HEADERS})
        get_filename_component(_dir ${_headerFile} PATH)
        list(APPEND INCLUDE_DIRS ${_dir})
    endforeach()
    list(REMOVE_DUPLICATES INCLUDE_DIRS)

    include_directories(${INCLUDE_DIRS})

    set(HEADERS
        ${HEADERS}
        ${FOUND_HEADERS}
        CACHE INTERNAL "headers" FORCE)
    list(REMOVE_DUPLICATES HEADERS)
endfunction(include_recursive_dirs)

function(check_and_link_libraries TARGET)
    foreach(_lib ${ARGN})
        string(TOUPPER ${_lib} LIB)
        find_package (${_lib})
        if(${LIB}_FOUND)
            include_directories(${${LIB}_INCLUDE_DIR})
            target_link_libraries(${TARGET} ${${LIB}_LIBRARIES})
        else()
            message(SEND_ERROR "You NEED ${_lib} library.")
            return()
        endif ()
    endforeach()
endfunction(check_and_link_libraries)

macro(add_bundle_resources RESOURCE_LIST RESOURCE_DIR RESOURCE_BASE)
    message("Adding Bundle Resources From: ${RESOURCE_DIR}")
    file(GLOB_RECURSE FULL_RESOURCE_PATHS "${RESOURCE_DIR}/[^.]**")
    foreach(_full_resource_path ${FULL_RESOURCE_PATHS})
        file(RELATIVE_PATH REL_RESOURCE_PATH ${RESOURCE_DIR} ${_full_resource_path})
        get_filename_component(REL_RESOURCE_DIR ${REL_RESOURCE_PATH} PATH)
        set_source_files_properties(${_full_resource_path} PROPERTIES MACOSX_PACKAGE_LOCATION "${RESOURCE_BASE}/${REL_RESOURCE_DIR}")
        message(STATUS "resource at: ${_full_resource_path}\n   remapped to: ${RESOURCE_BASE}/${REL_RESOURCE_DIR}")
    endforeach()
    list(APPEND ${RESOURCE_LIST} ${FULL_RESOURCE_PATHS})

endmacro(add_bundle_resources)

macro(add_resources TARGET RESOURCE_DIR DEST_DIR)

    add_custom_command(TARGET ${TARGET}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${RESOURCE_DIR} ${CMAKE_BINARY_DIR}/${DEST_DIR})

endmacro(add_resources)

macro(target_add_framework APPNAME FWNAME)
    find_library(${FWNAME}_FRAMEWORK ${FWNAME})
    if(${${FWNAME}_FRAMEWORK})
        target_link_libraries(${APPNAME} PRIVATE ${FRAMEWORK_${FWNAME}})
        message(STATUS "Framework ${FWNAME} found")
    else()
        message(SEND_ERROR "Framework ${FWNAME} not found")
    endif()
endmacro(target_add_framework)

# This little macro lets you set any XCode specific property
macro (set_xcode_property TARGET XCODE_PROPERTY XCODE_VALUE)
    set_property (TARGET ${TARGET} PROPERTY XCODE_ATTRIBUTE_${XCODE_PROPERTY} ${XCODE_VALUE})
endmacro (set_xcode_property)

