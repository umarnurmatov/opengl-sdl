cmake_minimum_required(VERSION 3.25)

project(
    opengl-test
    #LANGUAGES CXX
)

# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(cmake/ccache.cmake)


add_subdirectory(lib)
add_subdirectory(src)