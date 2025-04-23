QT += quick quickcontrols2 testlib
CONFIG += c++17 testcase
QT += core gui network qml quick charts
QT += concurrent

SOURCES += \
    mainwindow.cpp \
    storage.cpp \
    analysis.cpp \
    main.cpp \
    dataloaderworker.cpp \
    #testy/test_main.cpp \
    testy/test_analysis.cpp \
    testy/test_storage.cpp

HEADERS += \
    mainwindow.h \
    storage.h \
    analysis.h \
    dataloaderworker.h \
    testy/test_storage.h \
    testy/test_analysis.h

RESOURCES += qml.qrc


QML_IMPORT_PATH += $$[QT_INSTALL_QML]

# Ścieżka do folderu testów
VPATH += testy
INCLUDEPATH += testy


