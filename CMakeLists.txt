cmake_minimum_required(VERSION 3.1)

project(assertf.h.test)

set(CMAKE_C_STANDARD 99)

add_compile_options(-Wall -Wextra -g -DDEBUG -O0)
#add_compile_options(-Wall -Wextra -O2 -DASSERTF_DISABLE)

add_executable(
        ${PROJECT_NAME}
        assertf.h
        test.c
)
