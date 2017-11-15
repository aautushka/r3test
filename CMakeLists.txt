cmake_minimum_required(VERSION 2.8)

project(r3test)

add_compile_options(-std=gnu++14)

include_directories (/opt/range-v3/include)

set(SRC 
    main
)

add_executable(r3test ${SRC})

