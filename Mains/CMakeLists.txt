cmake_minimum_required(VERSION 3.10)
project(mains VERSION 1.0.0 LANGUAGES CXX)
include(GNUInstallDirs)

set(MAINS_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
file(GLOB MAINS_SOURCE ${MAINS_SOURCE_DIR}/*.cpp)

foreach(SOURCE_PATH ${MAINS_SOURCE})

    get_filename_component(EXECUTABLE_NAME ${SOURCE_PATH} NAME_WE)

    add_executable(${EXECUTABLE_NAME} ${SOURCE_PATH})

    target_compile_features(${EXECUTABLE_NAME} PUBLIC cxx_std_20)
    target_compile_options(${EXECUTABLE_NAME} PRIVATE -Wall -Wextra)

    #target_link_libraries(${EXECUTABLE_NAME} PRIVATE transport.shrmem-Server)
    #target_link_libraries(${EXECUTABLE_NAME} PRIVATE transport.shrmem-Client)
    target_link_libraries(${EXECUTABLE_NAME} PRIVATE transport.shrmem)

    set_target_properties(${EXECUTABLE_NAME} PROPERTIES VERSION ${PROJECT_VERSION})

endforeach(SOURCE_PATH ${MAINS_SOURCE})
