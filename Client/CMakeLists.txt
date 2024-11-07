cmake_minimum_required(VERSION 3.10)
project(ShrMemClient VERSION 1.0.0 LANGUAGES CXX)
include(GNUInstallDirs)

add_executable(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/main2.cpp)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
target_compile_options(${PROJECT_NAME} PRIVATE "-Wall")

target_link_libraries (${PROJECT_NAME} PRIVATE SharedMemory)

set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})