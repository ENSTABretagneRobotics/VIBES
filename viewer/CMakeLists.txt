CMAKE_MINIMUM_REQUIRED(VERSION 2.8.9)

PROJECT(VIBes_viewer)

# Automatically preprocess Qt classes with MOC
SET(CMAKE_AUTOMOC ON)
SET(CMAKE_INCLUDE_CURRENT_DIR ON)

# Force -fPIC flag, needed to use Qt5 on Linux.
if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -fPIC)
endif()

# Find Qt5 Modules
FIND_PACKAGE(Qt5Core REQUIRED)
FIND_PACKAGE(Qt5Widgets REQUIRED)
FIND_PACKAGE(Qt5Gui REQUIRED)

# Source files
SET(VIBes_viewer_SOURCES main.cpp figure2d.cpp vibestreemodel.cpp vibeswindow.cpp)

# Qt designer UI files
SET(VIBes_viewer_FORMS vibeswindow.ui)
# Process Qt UI files
QT5_WRAP_UI(VIBes_viewer_FORMS_HEADERS ${VIBes_viewer_FORMS})

# Vibes-viewer executable
ADD_EXECUTABLE(VIBes-viewer WIN32 MACOSX_BUNDLE ${VIBes_viewer_SOURCES} ${VIBes_viewer_FORMS_HEADERS})

# Qt5 Modules
QT5_USE_MODULES(VIBes-viewer Widgets Gui Core Svg)
