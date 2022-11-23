TEMPLATE = app
TARGET = VIBes_viewer
INCLUDEPATH += .

QT += widgets svg concurrent network

CONFIG += release static
#QTPLUGIN += svg
QMAKE_CXXFLAGS += -std=c++11
# Input
HEADERS +=  vibestreemodel.h vibeswindow.h figure2d.h vibesscene2d.h vibesgraphicsitem.h propertyeditdialog.h treeview.h
FORMS += vibeswindow.ui propertyeditdialog.ui
SOURCES += main.cpp vibestreemodel.cpp vibeswindow.cpp figure2d.cpp vibesscene2d.cpp vibesgraphicsitem.cpp propertyeditdialog.cpp treeview.cpp

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
