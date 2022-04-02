cmake_minimum_required(VERSION 3.0)
project(OpenGL)

file(GLOB SRC
"src/*.c*"
"include/*.hpp"
)

include_directories(include)
link_directories(lib)

add_executable(${PROJECT_NAME} ${SRC})

target_link_libraries(${PROJECT_NAME} glfw3)