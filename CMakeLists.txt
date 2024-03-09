cmake_minimum_required(VERSION 3.1)

project(bigintlib C)

set(SOURCES tests/maintest.c)
set(EXENAME bigint)

add_subdirectory(bigintlib)
add_executable(${EXENAME} ${SOURCES})

target_compile_options(${EXENAME} PRIVATE -Wall)
target_link_libraries(${EXENAME} PUBLIC bigintlib)
