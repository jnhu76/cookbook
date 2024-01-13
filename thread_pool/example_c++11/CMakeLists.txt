cmake_minimum_required(VERSION 3.5)

project(ThreadPool LANGUAGES CXX)
find_package(Threads REQUIRED)


# pthread support
set(THREADS_PREFER_PTHREAD_FLAG ON)

add_executable(test test.cpp)
target_compile_features(test PRIVATE cxx_std_17)
target_include_directories(test PRIVATE ${PROJECT_SOURCE_DIR})
target_link_libraries(test PRIVATE Threads::Threads)