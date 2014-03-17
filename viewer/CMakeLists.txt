CMAKE_MINIMUM_REQUIRED(VERSION 2.8.9)

PROJECT(VIBes_viewer)

# Viewer executable name
SET(VIBES_viewer_EXE VIBes-viewer)

SET(CMAKE_OSX_ARCHITECTURES x86_64) #;i386;ppc;ppc64)

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

# Build and add an application icon
include(MacroAddAppIcon.cmake)
ADD_APP_ICON(VIBes_viewer_SOURCES icons/vibes)

# Vibes-viewer executable
ADD_EXECUTABLE(${VIBES_viewer_EXE} WIN32 MACOSX_BUNDLE ${VIBes_viewer_SOURCES} ${VIBes_viewer_FORMS_HEADERS})

# Qt5 Modules
QT5_USE_MODULES(${VIBES_viewer_EXE} Widgets Gui Core Svg)

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
