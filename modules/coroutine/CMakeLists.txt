file(GLOB SRC_FILES *.c)
add_library(coroutine ${SRC_FILES})
target_include_directories(${CMAKE_SOURCE_DIR}/include)
