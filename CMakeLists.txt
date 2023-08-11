cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(Rasterizer)

file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp ${PROJECT_SOURCE_DIR}/src/*.h)
add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC
    ${PROJECT_SOURCE_DIR}/src
)

add_subdirectory(config)

add_subdirectory(third_party/glm)
set(SDL_STATIC ON)
add_subdirectory(third_party/SDL)
target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC config third_party/glm third_party/SDL/include third_party/stb/include)
target_link_libraries(${PROJECT_NAME} PUBLIC SDL2::SDL2-static)