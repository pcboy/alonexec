CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(alonexec)
SET(SRCS
    ../src/list.c
    ../src/utils.c
    ../src/alonexec.c
    ../src/main.c
    )
SET(CMAKE_C_FLAGS "-W -Wall -Wextra -O2")
INCLUDE_DIRECTORIES(../inc/ sexpr_1.2/src/)
LINK_DIRECTORIES(sexpr_1.2/src/)
ADD_EXECUTABLE(alonexec ${SRCS})
TARGET_LINK_LIBRARIES(alonexec sexp)
