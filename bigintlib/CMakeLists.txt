set(SOURCES bigint.c)
set(HEADERS bigint.h)

add_library(bigintlib STATIC ${SOURCES} ${HEADERS})

target_compile_options(bigintlib PRIVATE -Wall)
target_include_directories(bigintlib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
