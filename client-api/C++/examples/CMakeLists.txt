CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(vibes-cpp-examples)

ADD_EXECUTABLE(all_commands all_commands.cpp ${CMAKE_CURRENT_BINARY_DIR}/../../src/vibes.cpp)
