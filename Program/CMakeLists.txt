cmake_minimum_required(VERSION 3.14)
project(one-way-sync VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

#Boost
set(BOOST_ROOT "C:/CLI_STUFF/mingw64/boost_1_78_0/")
set(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "C:/CLI_STUFF/mingw64/boost_1_78_0/")
set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "C:/CLI_STUFF/mingw64/boost_1_78_0/lib/")

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost COMPONENTS system)

include_directories(${Boost_INCLUDE_DIR})

#Threads
find_package(Threads REQUIRED)

#FMT
FetchContent_Declare(fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG master
)
FetchContent_MakeAvailable(fmt)

#JSON-parsing
FetchContent_Declare(nlohmann_json
  GIT_REPOSITORY https://github.com/nlohmann/json
  GIT_TAG master
)
FetchContent_MakeAvailable(nlohmann_json)

set (source_dir "${PROJECT_SOURCE_DIR}/src/")

file (GLOB source_files 
    "${source_dir}/*.cpp"
    "${source_dir}/config/type/*.cpp" 
    "${source_dir}/config/*.cpp" 
    "${source_dir}/helpers/*.cpp" 
    "${source_dir}/models/*.cpp" 
    "${source_dir}/modules/*.cpp" 
    "${source_dir}/networking/*.cpp" 
    "${source_dir}/utils/*.cpp" 
)

include_directories(
    "${source_dir}/"
)

add_compile_options(-Wall -Wextra -pedantic -Werror -pthread)

add_executable(one-way-sync ${source_files})

target_link_libraries(one-way-sync ws2_32)
target_link_libraries(one-way-sync Threads::Threads)
target_link_libraries(one-way-sync fmt::fmt-header-only)
target_link_libraries(one-way-sync nlohmann_json::nlohmann_json)
