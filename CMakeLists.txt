cmake_minimum_required(VERSION 3.00)

set(CMAKE_EXPORT_COMPILE_COMMANDS on)
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_TOOLCHAIN_FILE "D:/other_things/Programs/cpps/develop/vcpkg/scripts/buildsystems/vcpkg.cmake")

# vcpkg配置
set(VCPKG_HOST_TRIPLET x64-windows)
set(VCPKG_TARGET_TRIPLET x64-windows)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w") # 禁用所有警告

# set(VCPKG_HOST_TRIPLET x86-mingw-dynamic)
# set(VCPKG_TARGET_TRIPLET x86-mingw-dynamic)

# 项目名称
project(main)

# 连接vcpkg
# 使用环境变量替换路径
if(DEFINED ENV{VCPKG_ROOT})
    set(VCPKG_ROOT $ENV{VCPKG_ROOT})
else()
    message(FATAL_ERROR "Environment variable VCPKG_ROOT is not set.")
endif()

# 使用环境变量指定 include 和 link 目录
include_directories(${VCPKG_ROOT}/installed/x64-windows/include)
link_directories(${VCPKG_ROOT}/installed/x64-windows/lib)

aux_source_directory(src SOURCES)
aux_source_directory(src/engine/core SOURCES)
aux_source_directory(src/engine/renderer SOURCES)

file(GLOB_RECURSE HEADERS "./include/*.hpp")
add_executable(main main.cpp ${SOURCES} ${HEADERS})
target_include_directories(main PUBLIC ./include)
target_compile_features(main PUBLIC cxx_std_17)

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(main PRIVATE glfw)

find_package(GLEW REQUIRED)
target_link_libraries(main PRIVATE GLEW::GLEW)

find_package(glm CONFIG REQUIRED)
target_link_libraries(main PRIVATE glm::glm)

# 将assets文件夹复制到输出目录
file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
