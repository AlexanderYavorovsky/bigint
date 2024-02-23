cmake_minimum_required(VERSION 3.22.1)

project(bigintlib C)

set(SOURCES bigint.c)
set(HEADERS bigint.h)

add_compile_options(-Wall)

add_library(bigintlib STATIC ${SOURCES} ${HEADERS})