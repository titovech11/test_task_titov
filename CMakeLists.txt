project(source)

file(GLOB SOURCES
    dav_class.h
    source.cpp
)

add_executable(source  ${SOURCES})

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/json
DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(source ${OpenCV_LIBS})