CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

IF (WIN32)
  ADD_DEFINITIONS(-DWIN32=1)
ELSE()
  ADD_DEFINITIONS(-DUNIX=1)
ENDIF()

PROJECT(vibes-cpp-examples)

SET(vibes_SOURCES ${CMAKE_CURRENT_LIST_DIR}/../src/vibes.cpp)

SET(interval_HEADERS interval/box.h interval/iboolean.h interval/interval.h)
SET(interval_SOURCES interval/box.cpp interval/iboolean.cpp interval/interval.cpp)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_LIST_DIR}/../src/)

ADD_EXECUTABLE(all_commands all_commands.cpp ${vibes_SOURCES})

INCLUDE_DIRECTORIES(interval)

ADD_EXECUTABLE(sivia_simple sivia_simple.cpp ${interval_SOURCES} ${vibes_SOURCES})

ADD_EXECUTABLE(pong pong.cpp ${interval_SOURCES} ${vibes_SOURCES})
