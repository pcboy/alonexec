CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(alonexec)
SET(CMAKE_BUILD_TYPE Release)
EXECUTE_PROCESS(COMMAND "sh" "${CMAKE_CURRENT_SOURCE_DIR}/dependencies.sh")
SET(SRCS
    ../src/cross.c
    ../src/list.c
    ../src/utils.c
    ../src/alonexec.c
    ../src/main.c
    )
SET(CMAKE_C_FLAGS "-W -Wall -Wextra")
INCLUDE_DIRECTORIES(../inc/ sexpr_1.2/src/)
LINK_DIRECTORIES(sexpr_1.2/src/)
ADD_EXECUTABLE(alonexec ${SRCS})
TARGET_LINK_LIBRARIES(alonexec sexp)

INSTALL(DIRECTORY ../rsrc/ DESTINATION share/alonexec/)
INSTALL(TARGETS alonexec DESTINATION bin)
INSTALL(PROGRAMS alonexecc DESTINATION bin)
