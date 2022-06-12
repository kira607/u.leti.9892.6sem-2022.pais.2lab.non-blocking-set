cmake_minimum_required(VERSION 3.15)
set(PROJECT_NAME 2lab_pas_3_2)
project(${PROJECT_NAME})
find_package(Threads REQUIRED)

set(THREADS_PREFER_PTHREAD_FLAG ON)
set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES 
    src/log.h
    src/main.cpp
    src/timer.h

    src/rand/rand.cpp
    src/rand/rand.h

    src/set/set.h
    src/set/set.cpp

    src/set/test_set/test_set.cpp
    src/set/test_set/test_set.h
    src/set/test_set/set_operations_distribution.cpp
    src/set/test_set/set_operations_distribution.h
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads)
