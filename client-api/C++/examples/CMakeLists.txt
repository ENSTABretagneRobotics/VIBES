CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(vibes-cpp-examples)

SET(interval_HEADERS interval/box.h interval/iboolean.h interval/interval.h)
SET(interval_SOURCES interval/box.cpp interval/iboolean.cpp interval/interval.cpp)

ADD_EXECUTABLE(all_commands all_commands.cpp ${CMAKE_CURRENT_LIST_DIR}/../src/vibes.cpp)

ADD_EXECUTABLE(sivia_simple sivia_simple.cpp ${interval_SOURCES})
