cmake_minimum_required(VERSION 3.28)
project(json)

set(CMAKE_CXX_STANDARD 20)

set(TARGET_NAME json)
file(GLOB_RECURSE TARGET_INC include/*)
file(GLOB_RECURSE TARGET_SRC src/*)
add_executable(${TARGET_NAME} ${TARGET_INC} ${TARGET_SRC})
target_include_directories(${TARGET_NAME} PRIVATE include)

option(JSON_DEFINE_OSTREAM "" OFF)
if (JSON_DEFINE_OSTREAM)
    message("${TARGET_NAME} will define ostream operators")
    target_compile_definitions(${TARGET_NAME} PRIVATE -DJSON_DEFINE_OSTREAM)
endif ()

option(JSON_EXIT_NOT_THROW "" OFF)
if (JSON_EXIT_NOT_THROW)
    message("${TARGET_NAME} will exit instead of throw")
    target_compile_definitions(${TARGET_NAME} PRIVATE -DJSON_EXIT_NOT_THROW)
endif ()

set(TARGET_NAME sandbox)
file(GLOB_RECURSE TARGET_INC include/*)
file(GLOB_RECURSE TARGET_SRC sandbox/*)
add_executable(${TARGET_NAME} ${TARGET_INC} ${TARGET_SRC})
target_include_directories(${TARGET_NAME} PRIVATE include)

option(JSON_DEFINE_OSTREAM "" OFF)
if (JSON_DEFINE_OSTREAM)
    message("${TARGET_NAME} will define ostream operators")
    target_compile_definitions(${TARGET_NAME} PRIVATE -DJSON_DEFINE_OSTREAM)
endif ()

option(JSON_EXIT_NOT_THROW "" OFF)
if (JSON_EXIT_NOT_THROW)
    message("${TARGET_NAME} will exit instead of throw")
    target_compile_definitions(${TARGET_NAME} PRIVATE -DJSON_EXIT_NOT_THROW)
endif ()
