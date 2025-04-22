cmake_minimum_required(VERSION 3.16)
project(C_http VERSION 1.0.0 LANGUAGES C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB SRC_FILES CONFIGURE_DEPENDS src/*.c)

add_executable(${CMAKE_PROJECT_NAME}
  main.c
  ${SRC_FILES}
)

add_subdirectory(modules)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE utils http)
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE include/)
target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE -Wall -Wextra)


