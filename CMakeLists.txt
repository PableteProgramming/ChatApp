cmake_minimum_required(VERSION 3.12)

project(ChatApp)

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)


add_executable(${PROJECT_NAME} ${SRC_DIR}/main.cpp)

target_include_directories(
	${PROJECT_NAME} PUBLIC
	${INCLUDE_DIR}	
)
