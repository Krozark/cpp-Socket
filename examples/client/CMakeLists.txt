set(HEADERS
    defines.hpp
    functions.hpp
)

set(SRC
    client.cpp
    functions.cpp
)

set(FILES ${HEADERS} ${SRC})

add_executable(${CMAKE_PROJECT_NAME}-client ${FILES})
target_link_libraries(${CMAKE_PROJECT_NAME}-client ${LIBS} socket)
