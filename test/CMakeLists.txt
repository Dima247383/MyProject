cmake_minimum_required(VERSION 3.15)
project(MyTests)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")

# FetchContent для GoogleTest
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.13.0
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include_directories(${CMAKE_SOURCE_DIR}/include) # Путь к exemple1.h
include_directories(${gtest_SOURCE_DIR}/googletest/include)
include_directories(${gtest_SOURCE_DIR}/googletest)

set(TEST_SOURCE_FILES test.cpp)

add_executable(MyTests ${TEST_SOURCE_FILES})

#target_link_libraries(MyTests gtest_main MyProject_Lib)
target_link_libraries(MyTests
    GTest::gtest_main
    MyProject_Lib  # Линкуем с исполняемым файлом MyProject
    nlohmann_json::nlohmann_json # Линкуем с nlohmann_json (если нужно)
)

include(GoogleTest)
enable_testing()
add_test(NAME UnitTests COMMAND MyTests)