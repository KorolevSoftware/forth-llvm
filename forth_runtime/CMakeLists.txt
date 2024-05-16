cmake_minimum_required(VERSION 3.14)
project(Forth_Runtime C)

set(SOURCES
  forth_runtime.c
  forth_runtime.h
)

list(TRANSFORM SOURCES PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/")

add_library(${PROJECT_NAME} STATIC ${SOURCES})

if(MSVC)
  target_compile_options(${PROJECT_NAME} PRIVATE
    -Wall
    # -WX
  )
endif()

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
