cmake_minimum_required(VERSION 3.14)
project(Forth-LLVM CXX)

include(CMakePrintHelpers)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_subdirectory(forth_runtime)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# LLVM
find_package(LLVM REQUIRED CONFIG)

llvm_map_components_to_libnames(llvm_libs support core irreader object native
  AllTargetsAsmParsers
  AllTargetsCodeGens
  AllTargetsDescs
  AllTargetsInfos)

set(AST_SOURCES

)

set(SOURCES
    main.cpp
    tokenizer.cpp
    parser.cpp
    tokenizer.h
    parser.h
    token.h
    token.cpp
    ast_entity.cpp
    ast_entity.h
    ast_builder.cpp
    ast_builder.h
    ast_codegen.cpp
    ast_codegen.h
    enviroment.h
)

list(TRANSFORM SOURCES PREPEND ${CMAKE_CURRENT_SOURCE_DIR}/src/)
list(TRANSFORM AST_SOURCES PREPEND ${CMAKE_CURRENT_SOURCE_DIR}/src/)

add_executable(${PROJECT_NAME} ${SOURCES} ${AST_SOURCES})

source_group("AST" FILES ${AST_SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE ${LLVM_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${llvm_libs} Forth_Runtime)




  # LLVM_LIBS
  # AllTargetsAsmParsers
  # AllTargetsCodeGens
  # AllTargetsDescs
  # AllTargetsInfos
  # AggressiveInstCombine
  # Analysis
  # AsmParser
  # BitWriter
  # CodeGen
  # Core
  # Extensions
  # IPO
  # IRReader
  # InstCombine
  # Instrumentation
  # MC
  # MCJIT
  # ObjCARCOpts
  # OrcJIT
  # Remarks
  # ScalarOpts
  # Support
  # Symbolize
  # Target
  # TransformUtils
  # Vectorize
  # Passes)