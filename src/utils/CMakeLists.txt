file(GLOB SRC_FILES *.c)
add_library(utils ${SRC_FILES})
target_include_directories(utils PUBLIC ${CMAKE_SOURCE_DIR}/include)
