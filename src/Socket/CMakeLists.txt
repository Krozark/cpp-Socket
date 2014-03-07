set(INCROOT ${PROJECT_SOURCE_DIR}/include/Socket)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/Socket)

set(CLIENT_SRC
    ${SRCROOT}/client/Client.cpp
    ${INCROOT}/client/Client.hpp
    ${INCROOT}/client/Client.tpl
)

set(SERVER_SRC
    ${SRCROOT}/server/Client.cpp
    ${INCROOT}/server/Client.hpp
    ${SRCROOT}/server/Server.cpp
    ${INCROOT}/server/Server.hpp
)

set(SRC
    ${SRCROOT}/BalancingSelector.cpp
    ${INCROOT}/BalancingSelector.hpp
    ${SRCROOT}/Config.cpp
    ${INCROOT}/Config.hpp
    ${SRCROOT}/FuncWrapper.cpp
    ${INCROOT}/FuncWrapper.hpp
    ${INCROOT}/FuncWrapper.tpl
    ${INCROOT}/define.hpp
    ${SRCROOT}/SelectManager.cpp
    ${INCROOT}/SelectManager.hpp
    ${SRCROOT}/Serializer.cpp
    ${INCROOT}/Serializer.hpp
    ${SRCROOT}/Socket.cpp
    ${INCROOT}/Socket.hpp
    ${SRCROOT}/SocketSerialized.cpp
    ${INCROOT}/SocketSerialized.hpp
)

add_library(socket SHARED ${SRC} ${CLIENT_SRC} ${SERVER_SRC})

SET_TARGET_PROPERTIES(socket
    PROPERTIES
    VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}
    SOVERSION ${VERSION_MAJOR}.${VERSION_MINOR})

# Installation of the library
INSTALL(TARGETS socket
    DESTINATION lib
    PERMISSIONS OWNER_READ GROUP_READ WORLD_READ)

add_library(socket-static STATIC ${SRC} ${CLIENT_SRC} ${SERVER_SRC})

SET_TARGET_PROPERTIES(socket-static
    PROPERTIES
    VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}
    SOVERSION ${VERSION_MAJOR}.${VERSION_MINOR})

# Installation of the library
INSTALL(TARGETS socket-static
    DESTINATION lib
    PERMISSIONS OWNER_READ GROUP_READ WORLD_READ)
