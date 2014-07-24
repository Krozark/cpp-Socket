add_executable(broadcast-client broadcast-client.cpp)
target_link_libraries(broadcast-client ${LIBS} socket)

add_executable(broadcast-server broadcast-server.cpp)
target_link_libraries(broadcast-server ${LIBS} socket)

