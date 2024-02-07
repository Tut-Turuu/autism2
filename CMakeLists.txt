cmake_minimum_required(VERSION 3.15)



set(PROJECT_NAME Autism2) 	 
project("${PROJECT_NAME}")


set(CMAKE_CXX_STANDARD 17)			
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

include_directories("src/" "inc/")

set(
    SOURCES
    main.cpp
    cube.cpp
)

set(
    HEADERS
    cube.hpp
)

add_compile_options(
    # -Werror
    # -Wall
    # -Wextra
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_link_libraries(${PROJECT_NAME} PRIVATE sfml-graphics sfml-window sfml-system)