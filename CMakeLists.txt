cmake_minimum_required(VERSION 3.15)
project(MyProject)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
# 1. FetchContent для nlohmann_json
include(FetchContent)
FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.3 # Или другая подходящая версия (рекомендуется использовать конкретный тег)
)
FetchContent_MakeAvailable(nlohmann_json)

add_subdirectory(test)

include_directories(include)

set(SOURCE_FILES
	src/main.cpp
	src/ConverterJSON.cpp
	src/InvertedIndex.cpp
	src/SearchServer.cpp
)

add_executable(MyProject ${SOURCE_FILES})

target_link_libraries(MyProject PRIVATE nlohmann_json::nlohmann_json)

add_library(MyProject_Lib STATIC ${SOURCE_FILES})
target_link_libraries(MyProject_Lib PUBLIC)
target_link_libraries(MyProject_Lib PRIVATE nlohmann_json::nlohmann_json)

