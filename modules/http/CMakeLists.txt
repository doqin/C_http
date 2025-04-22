file(GLOB SRC_FILES *.c)
add_library(http ${SRC_FILES})
target_include_directories(http PUBLIC ${CMAKE_SOURCE_DIR}/include)
