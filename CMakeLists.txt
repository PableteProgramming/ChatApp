cmake_minimum_required(VERSION 3.12)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "ON if dll. OFF if static" FORCE)
set(SFML_USE_STATIC_STD_LIBS ON CACHE BOOL "ON if static. OFF if dll" FORCE)

project(TicTacToe)

find_package(OpenGL REQUIRED)

set(SFML_DIRECTORY SFML)
set(SFML_INCLUDE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/${SFML_DIRECTORY}/include)

add_subdirectory(${SFML_DIRECTORY})

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(${PROJECT_NAME}_PLATLIBS freetype jpeg)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set(${PROJECT_NAME}PLATLIBS winmm gdi32)
endif()

set(LIBS sfml-graphics sfml-window sfml-system sfml-audio ${OPENGL_LIBRARY} ${${PROJECT_NAME}_PLATLIBS} ${SFML_LIBRARIES})

add_subdirectory(${PROJECT_NAME})
