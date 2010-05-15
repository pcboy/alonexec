CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(alonexec)
EXECUTE_PROCESS(COMMAND "sh" "${CMAKE_CURRENT_SOURCE_DIR}/dependencies.sh")
SET(SRCS
    ../src/list.c
    ../src/utils.c
    ../src/alonexec.c
    ../src/main.c
    )
SET(CMAKE_C_FLAGS "-W -Wall -Wextra -O2 -g3")
INCLUDE_DIRECTORIES(../inc/ sexpr_1.2/src/)
LINK_DIRECTORIES(sexpr_1.2/src/)
ADD_EXECUTABLE(alonexec ${SRCS})
TARGET_LINK_LIBRARIES(alonexec sexp)
