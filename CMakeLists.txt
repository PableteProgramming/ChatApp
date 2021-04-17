cmake_minimum_required(VERSION 3.12)

project(ChatApp)

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)


add_executable(server ${SRC_DIR}/server.cpp)

target_include_directories(
	server PUBLIC
	${INCLUDE_DIR}	
)

add_executable(client ${SRC_DIR}/client.cpp)

target_include_directories(
	client PUBLIC
	${INCLUDE_DIR}	
)
