cmake_minimum_required(VERSION 3.22.1)

project(testmain C)

set(SOURCES tests/maintest.c)
set(SUBDIR bigintlib)

add_compile_options(-Wall)

add_subdirectory(${SUBDIR})

add_executable(testmain ${SOURCES})

target_link_libraries(testmain PUBLIC bigintlib)

target_include_directories(testmain PUBLIC "${SUBDIR}")