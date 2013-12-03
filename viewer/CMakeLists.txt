cmake_minimum_required(VERSION 2.8)

PROJECT(VIBes_viewer)
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

FIND_PACKAGE(Qt5Core REQUIRED)
FIND_PACKAGE(Qt5Widgets REQUIRED)
FIND_PACKAGE(Qt5Gui REQUIRED)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
INCLUDE_DIRECTORIES(${Qt5Core_INCLUDES} ${Qt5Widgets_INCLUDES} ${Qt5Gui_INCLUDES})
ADD_DEFINITIONS(${Qt5Core_DEFINITIONS} ${Qt5Widgets_DEFINITIONS} ${Qt5Gui_DEFINITIONS})

SET(VIBes_viewer_SOURCES main.cpp figure2d.cpp vibestreemodel.cpp vibeswindow.cpp)
set(VIBes_viewer_FORMS vibeswindow.ui)

qt5_wrap_ui(VIBes_viewer_FORMS_HEADERS ${VIBes_viewer_FORMS})

ADD_EXECUTABLE(VIBes-viewer ${VIBes_viewer_SOURCES} ${VIBes_viewer_FORMS_HEADERS})
qt5_use_modules(VIBes-viewer Widgets Gui Core)
