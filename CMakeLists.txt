cmake_minimum_required(VERSION 3.28)
project(main)

set(CMAKE_CXX_STANDARD 17)

aux_source_directory(src SOURCES)
aux_source_directory(src/engine/core SOURCES)
aux_source_directory(src/engine/renderer SOURCES)
aux_source_directory(src/engine/node SOURCES)
aux_source_directory(src/engine/editor SOURCES)
aux_source_directory(src/engine/util SOURCES)
aux_source_directory(src/engine/event SOURCES)
aux_source_directory(src/engine/physics2D SOURCES)

file(GLOB_RECURSE HEADERS "./include/*.hpp")
add_executable(${PROJECT_NAME} main.cpp ${SOURCES} ${HEADERS})

target_include_directories(${PROJECT_NAME} PUBLIC ./include)


find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)

find_package(imgui CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE imgui::imgui)

find_package(glm CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE glm::glm-header-only)

find_package(glad CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE glad::glad)

find_package(rttr CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE RTTR::Core)

find_package(box2d CONFIG REQUIRED)
target_link_libraries(main PRIVATE box2d::box2d)

# 将assets文件夹复制到输出目录
file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})