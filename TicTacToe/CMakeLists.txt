cmake_minimum_required(VERSION 3.12)

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(server ${SRC_DIR}/server.cpp)

target_include_directories(
	server PUBLIC
	${INCLUDE_DIR}	
)

target_link_options(
	server PUBLIC
	-pthread
)

target_sources(
	server PUBLIC
	${SRC_DIR}/ClientClass.cpp	
	${SRC_DIR}/WaitingClient.cpp
)



add_executable(client ${SRC_DIR}/client.cpp)

target_include_directories(
	client PUBLIC
	${INCLUDE_DIR}	
)

target_link_options(
	client PUBLIC
	-pthread
)

add_executable(window ${SRC_DIR}/window.cpp)

target_include_directories(
	window PUBLIC
	${INCLUDE_DIR}	
	${SFML_INCLUDE_PATH}
)

target_link_libraries(
	window PUBLIC
	${LIBS}
)
