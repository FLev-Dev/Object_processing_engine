QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

HEADERS += \
        include/data_view.h \
        include/object_processing_engine.h \
        include/new_object_window.h \
        include/file_manager.h \
        include/sort_window.h \
        include/exit_button.h \
        include/mainwindow.h

FORMS += \
        other/mainwindow.ui

SOURCES += \
        main.cpp \
        src/data_view.cpp \
        src/exit_button.cpp \
        src/file_manager.cpp \
        src/mainwindow.cpp \
        src/new_object_window.cpp \
        src/object_processing_engine.cpp \
        src/sort_window.cpp

RESOURCES += \
    other/Error_log

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
