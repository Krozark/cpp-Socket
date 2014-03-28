set(HEADERS
    dispatch.hpp
    functions.hpp
)

set(SRC
    dispatch.cpp
    functions.cpp
    server.cpp
)

set(FILES ${HEADERS} ${SRC})

add_executable(${CMAKE_PROJECT_NAME}-server ${FILES})
target_link_libraries(${CMAKE_PROJECT_NAME}-server ${LIBS} socket)
