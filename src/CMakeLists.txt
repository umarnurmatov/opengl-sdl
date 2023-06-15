set(NAME opengl-test-main)

file(GLOB_RECURSE source_files
    "*.cpp")

add_executable(${NAME} 
    ${source_files} 
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp 
    ${IMGUI_DIR}/backends/imgui_impl_sdl2.cpp
    ${IMGUI_DIR}/imgui.cpp 
    ${IMGUI_DIR}/imgui_draw.cpp 
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/imgui_tables.cpp 
    ${IMGUI_DIR}/imgui_widgets.cpp)

target_include_directories(${NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Base
    ${CMAKE_CURRENT_SOURCE_DIR}/Camera
    ${CMAKE_CURRENT_SOURCE_DIR}/MySDL
    ${CMAKE_CURRENT_SOURCE_DIR}/Render
    ${CMAKE_CURRENT_SOURCE_DIR}/Utils)


find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

find_package(SDL2_image REQUIRED)
include_directories(${SDL2_IMAGE_INCLUDE_DIRS} ${IMGUI_DIR} ${IMGUI_DIR}/backends)

target_link_libraries(${NAME} PRIVATE
    ${SDL2_LIBRARIES}
    SDL2_image::SDL2_image
    glad
    glm
    assimp)

target_compile_definitions(${NAME} PUBLIC MY_SHADER_PATH="${CMAKE_SOURCE_DIR}/shaders/")
target_compile_definitions(${NAME} PUBLIC MY_RESOURCE_PATH="${CMAKE_SOURCE_DIR}/res/")