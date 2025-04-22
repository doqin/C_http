cmake_minimum_required(VERSION 3.16)
project(C_http VERSION 1.0.0 LANGUAGES C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(${CMAKE_PROJECT_NAME}
  main.c
)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE utils http)
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE include/)
add_subdirectory(src/)
target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE -Wall -Wextra)


