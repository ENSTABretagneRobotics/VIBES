TEMPLATE = app
TARGET = VIBes_viewer
INCLUDEPATH += .

QT += core widgets gui network svg

CONFIG += release static
#QTPLUGIN += svg
greaterThan(QT_MAJOR_VERSION, 4) {
 CONFIG += c++11
}
else {
 QMAKE_CXXFLAGS += -std=c++0x
}
# Input
HEADERS +=  vibestreemodel.h vibeswindow.h figure2d.h vibesscene2d.h vibesgraphicsitem.h propertyeditdialog.h
FORMS += vibeswindow.ui propertyeditdialog.ui
SOURCES += main.cpp vibestreemodel.cpp vibeswindow.cpp figure2d.cpp vibesscene2d.cpp vibesgraphicsitem.cpp propertyeditdialog.cpp

# Application icon
win32:RC_FILE += icons/vibes.rc

# Special config for static build
contains(CONFIG, static): {
      ######### on main.cpp use defined ########
      DEFINES += _USE_STATIC_BUILDS_

       exists($$[QT_INSTALL_PLUGINS]/imageformats/libqsvg.a) {
           QT += svg
           QTPLUGIN += qsvg
           DEFINES += _USE_qsvg
       }
   }
target.path = $$PREFIX/bin
INSTALLS += target
