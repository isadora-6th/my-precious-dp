cmake_minimum_required(VERSION 3.0.0)
project(one-way-sync VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)

set (source_dir "${PROJECT_SOURCE_DIR}/src/")

SET(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "C:/CLI_STUFF/mingw64/boost_1_78_0/")
SET(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "C:/CLI_STUFF/mingw64/boost_1_78_0/lib/")

set(BOOST_ROOT "C:/CLI_STUFF/mingw64/boost_1_78_0/")
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost COMPONENTS system)
find_package( Threads )

include_directories(${Boost_INCLUDE_DIR})

file (GLOB source_files 
    "${source_dir}/*.cpp"
    "${source_dir}/helpers/*.cpp" 
    "${source_dir}/models/*.cpp" 
    "${source_dir}/networking/*.cpp" 
)

include_directories(
    "${source_dir}/"
)

add_compile_options(-Wall -Wextra -pedantic -Werror -pthread)

add_executable(one-way-sync ${source_files})

target_link_libraries(one-way-sync ws2_32)
target_link_libraries(one-way-sync Threads::Threads)
