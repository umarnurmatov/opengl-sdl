set(NAME opengl-test-main)

file(GLOB_RECURSE source_files
    "*.cpp")

add_executable(${NAME} ${source_files})

target_include_directories(${NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/)


find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

find_package(SDL2_image REQUIRED)
include_directories(${SDL2_IMAGE_INCLUDE_DIRS})

target_link_libraries(${NAME} PRIVATE
    ${SDL2_LIBRARIES}
    SDL2_image::SDL2_image
    glad
    glm)

target_compile_definitions(${NAME} PUBLIC MY_SHADER_PATH="${CMAKE_SOURCE_DIR}/shaders/")
target_compile_definitions(${NAME} PUBLIC MY_RESOURCE_PATH="${CMAKE_SOURCE_DIR}/res/")