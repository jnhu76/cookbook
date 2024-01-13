cmake_minimum_required(VERSION 3.3)

project(pool LANGUAGES CXX)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -ggdb -Wall -Wpedantic -Wextra -Wconversion -Weffc++")

# pthread support
set(THREADS_PREFER_PTHREAD_FLAG ON)

# CMake Error at CMakeLists.txt:8 (add_executable):
#   Target "test" links to target "Threads::Threads" but the target was not
#   found.  Perhaps a find_package() call is missing for an IMPORTED target, or
#   an ALIAS target is missing?

set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)


add_executable(test test.cpp)
target_compile_features(test PUBLIC cxx_std_17)
target_include_directories(test PUBLIC ${PROJECT_SOURCE_DIR})
target_link_libraries(test PUBLIC Threads::Threads)