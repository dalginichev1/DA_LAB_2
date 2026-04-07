cmake_minimum_required(VERSION 3.10)
project(RBTreeTests)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")

# Google Test
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
FetchContent_MakeAvailable(googletest)

# Тестируемая программа

# Тесты
add_executable(rbtree_tests test.cpp)
add_executable(rbtree main.cpp)

find_library(DMALLOC_LIB dmalloc)
if(DMALLOC_LIB)
    message(STATUS "dmalloc found: ${DMALLOC_LIB}")
    target_link_libraries(rbtree PRIVATE ${DMALLOC_LIB})
    target_compile_definitions(rbtree PRIVATE DMALLOC)
endif()

target_link_libraries(rbtree_tests gtest_main gtest)
target_include_directories(rbtree_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

find_package(Threads REQUIRED)

target_link_libraries(rbtree PRIVATE profiler tcmalloc_and_profiler Threads::Threads)
target_link_libraries(rbtree PRIVATE profiler Threads::Threads)

enable_testing()
add_test(NAME RBTreeTests COMMAND rbtree_tests)