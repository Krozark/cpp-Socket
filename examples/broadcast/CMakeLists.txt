add_executable(broadcast-client broadcast-client.cpp)
target_link_libraries(broadcast-client socket)

add_executable(broadcast-server broadcast-server.cpp)
target_link_libraries(broadcast-server socket)

