cmake_minimum_required(VERSION 3.6)
project(OpenGL_Test)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(SOURCE_FILES
    dump_png.cpp
    main.cpp
    Makefile
    mat.h
    minigl.cpp
    minigl.h
    minigl.h.backup.h
    parse.cpp)

add_executable(OpenGL_Test ${SOURCE_FILES})

IF(APPLE)
    INCLUDE_DIRECTORIES ( /System/Library/Frameworks )
    FIND_LIBRARY(COCOA_LIBRARY Cocoa)
    FIND_LIBRARY(GLUT_LIBRARY GLUT )
    FIND_LIBRARY(OpenGL_LIBRARY OpenGL )
    MARK_AS_ADVANCED (COCOA_LIBRARY
            GLUT_LIBRARY
            OpenGL_LIBRARY)
    SET(EXTRA_LIBS ${COCOA_LIBRARY} ${GLUT_LIBRARY} ${OpenGL_LIBRARY} -lpng)
ENDIF (APPLE)

target_link_libraries(OpenGL_Test ${EXTRA_LIBS})