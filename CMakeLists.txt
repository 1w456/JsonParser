cmake_minimum_required(VERSION 3.10) # The minimum CMake version required is 3.10
project(JsonParser VERSION 1.0.1 LANGUAGES CXX)	

set (CMAKE_CXX_STANDARD 17) # Set the C++ standard to C++17

configure_file(${CMAKE_SOURCE_DIR}/JsonParserConfig.h.in JsonParserConfig.h)

# file(GLOB Sour "src/*.cpp")

set(SRC ../src/main.cpp ../src/JObject.cpp ../src/JsonParser.cpp)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
add_executable(${PROJECT_NAME}  ${SRC})  

target_include_directories(${PROJECT_NAME} PUBLIC 
                           ${CMAKE_SOURCE_DIR}/include
                           ${PROJECT_BINARY_DIR}              
                           ) 
