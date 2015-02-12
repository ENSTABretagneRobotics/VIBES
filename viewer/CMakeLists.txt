CMAKE_MINIMUM_REQUIRED(VERSION 2.8.9)

PROJECT(VIBes_viewer)

# Viewer executable name
SET(VIBES_viewer_EXE VIBes-viewer)

# Target architectures for MacOS X build
SET(CMAKE_OSX_ARCHITECTURES x86_64) #;i386;ppc;ppc64)

# Automatically preprocess Qt classes with MOC
SET(CMAKE_AUTOMOC ON)
SET(CMAKE_INCLUDE_CURRENT_DIR ON)

# We need C++11 (or C++0x)
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
    message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

# Force -fPIC flag, needed to use Qt5 on Linux.
if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif()

# Find Qt5 Modules
FIND_PACKAGE(Qt5Core REQUIRED)
FIND_PACKAGE(Qt5Widgets REQUIRED)
FIND_PACKAGE(Qt5Gui REQUIRED)
FIND_PACKAGE(Qt5Network REQUIRED)
FIND_PACKAGE(Qt5Svg REQUIRED)

# Source files
SET(VIBes_viewer_SOURCES main.cpp
                         vibesgraphicsitem.cpp
                         vibesscene2d.cpp
                         figure2d.cpp
                         vibestreemodel.cpp
                         vibeswindow.cpp
                         propertyeditdialog.cpp )

# Headers
SET(VIBes_viewer_HEADERS
                         vibesgraphicsitem.h
                         vibesscene2d.h
                         figure2d.h
                         vibestreemodel.h
                         vibeswindow.h
                         propertyeditdialog.h )

# Qt designer UI files
SET(VIBes_viewer_FORMS vibeswindow.ui propertyeditdialog.ui)
# Process Qt UI files
QT5_WRAP_UI(VIBes_viewer_FORMS_HEADERS ${VIBes_viewer_FORMS})

# Organise source files
source_group("Headers" FILES ${VIBes_viewer_HEADERS})
source_group("Qt generated headers" FILES ${VIBes_viewer_FORMS_HEADERS})

# Build and add an application icon
include(MacroAddAppIcon.cmake)
ADD_APP_ICON(VIBes_viewer_SOURCES icons/vibes)

# Vibes-viewer executable
ADD_EXECUTABLE(${VIBES_viewer_EXE} WIN32 MACOSX_BUNDLE ${VIBes_viewer_SOURCES} ${VIBes_viewer_HEADERS} ${VIBes_viewer_FORMS_HEADERS})

# Qt5 Modules
QT5_USE_MODULES(${VIBES_viewer_EXE} Widgets Gui Core Network Svg)

# Packaging
IF(APPLE)
    IF(NOT QT_BINARY_DIR)
        SET(QT_BINARY_DIR ${_qt5Core_install_prefix}/bin)
        message(STATUS "QT_BINARY_DIR was not set. Located it at ${QT_BINARY_DIR}")
    ENDIF(NOT QT_BINARY_DIR)

    # Copy dependencies to the app bundle
    INSTALL(CODE "message(STATUS \"Running ${QT_BINARY_DIR}/macdeployqt on ${VIBES_viewer_EXE}.app\")
                  execute_process(COMMAND ${QT_BINARY_DIR}/macdeployqt ${VIBES_viewer_EXE}.app)
                  ")

    # Copy the Application bundle
    INSTALL(TARGETS ${VIBES_viewer_EXE} BUNDLE DESTINATION .)#${CMAKE_CURRENT_BINARY_DIR})

    # Copy API files
    install(FILES ../client-api/C/vibes.h ../client-api/C/vibes.c DESTINATION "Vibes C")
    install(DIRECTORY ../client-api/C/examples DESTINATION "Vibes C")

    install(DIRECTORY ../client-api/C++/src DESTINATION "Vibes C++")
    install(DIRECTORY ../client-api/C++/examples DESTINATION "Vibes C++")

    # Setup DMG window appearance: background image, size and icon layout
    SET(CPACK_DMG_BACKGROUND_IMAGE ${CMAKE_CURRENT_SOURCE_DIR}/VIBes_DMG_bg/VIBes_DMG_bg.jpg)
    SET(CPACK_DMG_DS_STORE ${CMAKE_CURRENT_SOURCE_DIR}/VIBes_DMG_bg/DS_Store)

    SET(CPACK_PACKAGE_NAME VIBes)

    SET(CPACK_GENERATOR "DragNDrop")
    include(CPack)
ENDIF(APPLE)
