# This file is part of VIBes' C++ API examples
#
# Copyright (c) 2013 Jeremy Nicola
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.

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
