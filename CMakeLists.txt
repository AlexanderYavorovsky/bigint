cmake_minimum_required(VERSION 3.22.1)

project(maintest C)

set(SOURCES tests/maintest.c)
set(SUBDIR bigintlib)
set(EXENAME maintest)

add_compile_options(-Wall)

add_subdirectory(${SUBDIR})
add_executable(${EXENAME} ${SOURCES})

target_link_libraries(${EXENAME} PUBLIC bigintlib)
target_include_directories(${EXENAME} PUBLIC ${SUBDIR})