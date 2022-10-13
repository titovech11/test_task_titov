cmake_minimum_required(VERSION 3.8)
set (CMAKE_CXX_STANDARD 17)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "cmake")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/bin.dbg)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/bin.rel)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_CURRENT_SOURCE_DIR}/bin.relwithdbg)

project(test_task_titov)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})


project(test_task_titov)

file(GLOB SOURCES
    dav_class.h
    source.cpp
)

add_executable(test_task_titov  ${SOURCES})

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/json
DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Test.dav
DESTINATION ${CMAKE_CURRENT_BINARY_DIR})


target_link_libraries(test_task_titov ${OpenCV_LIBS})

