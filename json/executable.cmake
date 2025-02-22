execute_python("${TARGET_DIR}/generator.py")

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set_target_properties(${TARGET_NAME} PROPERTIES INTERPROCEDURAL_OPTIMIZATION ON)
    target_compile_options(
            ${TARGET_NAME} PRIVATE
            -O3
            -flto
            -march=native
            -funroll-loops
            -finline-functions
            -foptimize-sibling-calls
            -fno-exceptions
            -fno-rtti
    )
endif ()
