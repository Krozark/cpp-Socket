set(HEADERS
)

set(SRC
    socket_serializer.cpp
)

set(FILES ${HEADERS} ${SRC})

add_executable(test-socket_serializer ${FILES})
target_link_libraries(test-socket_serializer ${LIBS} socket)
