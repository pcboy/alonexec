CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(alonexec)
SET(SRCS
    ../src/list.c
    ../src/utils.c
    ../src/alonexec.c
    ../src/main.c
    )
SET(CMAKE_C_FLAGS "-W -Wall -Wextra -g3")
INCLUDE_DIRECTORIES(../inc/)
ADD_EXECUTABLE(alonexec ${SRCS})
