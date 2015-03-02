set(HEADERS
)

set(SRC
    serializer.cpp
)

set(FILES ${HEADERS} ${SRC})

add_executable(test-serializer ${FILES})
target_link_libraries(test-serializer ${LIBS} socket)
