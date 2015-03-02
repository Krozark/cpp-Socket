set(INCROOT ${PROJECT_SOURCE_DIR}/include/Socket)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/Socket)


#ROOT files
set(ROOT_SRC
    ${SRCROOT}/BalancingSelector.cpp
    ${SRCROOT}/FuncWrapper.cpp
    ${SRCROOT}/SelectManager.cpp
    ${SRCROOT}/Serializer.cpp
    ${SRCROOT}/Socket.cpp
    ${SRCROOT}/SocketSerialized.cpp
    ${SRCROOT}/Status.cpp
)
set(ROOT_HEADER
    ${INCROOT}/BalancingSelector.hpp
    ${INCROOT}/FuncWrapper.hpp
    ${INCROOT}/FuncWrapper.tpl
    ${INCROOT}/define.hpp
    ${INCROOT}/SelectManager.hpp
    ${INCROOT}/Serializer.hpp
    ${INCROOT}/Serializer.tpl
    ${INCROOT}/Socket.hpp
    ${INCROOT}/SocketSerialized.hpp
    ${INCROOT}/Status.hpp
)
set(ROOT_FILES
    ${ROOT_HEADER}
    ${ROOT_SRC}
)

#client sub dir
set(CLIENT_HEADER
    ${INCROOT}/client/Client.hpp
    ${INCROOT}/client/Client.tpl
)
set(CLIENT_SRC
    ${SRCROOT}/client/Client.cpp
)
set(CLIENT_FILES
    ${CLIENT_HEADER}
    ${CLIENT_SRC}
)


#server sub dir
set(SERVER_HEADER
    ${INCROOT}/server/Client.hpp
    ${INCROOT}/server/Server.hpp
)
set(SERVER_SRC
    ${SRCROOT}/server/Client.cpp
    ${SRCROOT}/server/Server.cpp
)
set(SERVER_FILES
    ${SERVER_HEADER}
    ${SERVER_SRC}
)

#FILES
set(FILES_HEADER
    ${ROOT_HEADER}
    ${CLIENT_HEADER}
    ${SERVER_HEADER}
)
set(FILES_SRC
    ${ROOT_SRC}
    ${CLIENT_SRC}
    ${SERVER_SRC}
)


#SHARED lib
add_library(socket SHARED ${FILES_HEADER} ${FILES_SRC})
SET_TARGET_PROPERTIES(socket
    PROPERTIES
    VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}
    SOVERSION ${VERSION_MAJOR}.${VERSION_MINOR}
)
target_link_libraries(socket ${LIBS})
INSTALL(TARGETS socket
    DESTINATION lib
)

#static lib
if(UNIX)
	add_library(socket-static STATIC ${FILES_HEADER} ${FILES_SRC})
	SET_TARGET_PROPERTIES(socket-static
		PROPERTIES
		VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}
		SOVERSION ${VERSION_MAJOR}.${VERSION_MINOR}
	)
	target_link_libraries(socket-static ${LIBS})
	INSTALL(TARGETS socket-static
		DESTINATION lib
	)
endif()

#header
#INSTALL(FILES ${INCROOT}
#    DESTINATION include
#    PERMISSIONS OWNER_READ GROUP_READ WORLD_READ
#)

file(GLOB DEPLOY_FILES_AND_DIRS "${PROJECT_SOURCE_DIR}/include/*")
foreach(ITEM ${DEPLOY_FILES_AND_DIRS})
   IF( IS_DIRECTORY "${ITEM}" )
      LIST( APPEND DIRS_TO_DEPLOY "${ITEM}" )
   ELSE()
      LIST( APPEND FILES_TO_DEPLOY "${ITEM}" )
   ENDIF()
endforeach()
INSTALL( FILES ${FILES_TO_DEPLOY} DESTINATION include )
INSTALL( DIRECTORY ${DIRS_TO_DEPLOY} DESTINATION include )

