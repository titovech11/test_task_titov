project(test_task_titov)
add_executable(test_task_titov source.cpp)

target_link_libraries(test_task_titov ${OpenCV_LIBS})

