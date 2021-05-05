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
	${SRC_DIR}/main.cpp
)

add_executable(client ${SRC_DIR}/client.cpp)

target_include_directories(
	client PUBLIC
	${SFML_INCLUDE_PATH}	
	${INCLUDE_DIR}
)

target_link_options(
	client PUBLIC
	-pthread
)

target_sources(
	client PUBLIC
	${SRC_DIR}/Player.cpp
	${SRC_DIR}/main.cpp
)

target_link_libraries(
	client PUBLIC
	${LIBS}
)

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
	target_compile_options(
        client PUBLIC
        "/MTd"
    )
endif()