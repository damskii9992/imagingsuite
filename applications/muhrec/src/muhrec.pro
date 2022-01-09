#-------------------------------------------------
#
# Project created by QtCreator 2013-04-07T13:45:32
#
#-------------------------------------------------

QT       += core widgets printsupport charts concurrent

TARGET = muhrec
VERSION = 4.3
TEMPLATE = app
CONFIG += c++11

DEFINES += VERSION=\\\"$$VERSION\\\"

CONFIG(release, debug|release):    DESTDIR = $$PWD/../../../../Applications
else:CONFIG(debug, debug|release): DESTDIR = $$PWD/../../../../Applications/debug

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target

    unix:macx {
        QMAKE_CXXFLAGS += -fPIC -O2
        INCLUDEPATH += /opt/local/include

        INCLUDEPATH += /opt/local/include/libxml2
        QMAKE_LIBDIR += /opt/local/lib
    }
    else {
        QMAKE_CXXFLAGS += -fPIC -fopenmp -O2
        QMAKE_LFLAGS += -lgomp
        LIBS += -lgomp
        INCLUDEPATH += /usr/include/libxml2
        LIBS += -L/usr/local/lib64 -L/usr/lib/x86_64-linux-gnu -lNeXus -lNeXusCPP
    }

    LIBS += -ltiff -lxml2 -larmadillo
}

win32 {
    contains(QMAKE_HOST.arch, x86_64):{
        QMAKE_LFLAGS += /MACHINE:X64
    }

    INCLUDEPATH  += $$PWD/../../../../ExternalDependencies/windows/include
    INCLUDEPATH  += $$PWD/../../../../ExternalDependencies/windows/include/libxml2
    INCLUDEPATH  += $$PWD/../../../../ExternalDependencies/windows/include/cfitsio
    QMAKE_LIBDIR += $$PWD/../../../../ExternalDependencies/windows/lib

    INCLUDEPATH  += $$PWD/../../../external/include
    QMAKE_LIBDIR += $$PWD/../../../external/lib64

    LIBS += -llibxml2 -llibtiff -lcfitsio
    QMAKE_CXXFLAGS += /openmp /O2
}

ICON = muh4_icon.icns
RC_ICONS = muh4_icon.ico

SOURCES += main.cpp\
    moduleconfigprogressdialog.cpp \
    muhrecmainwindow.cpp \
    MuhrecInteractor.cpp \
    configuregeometrydialog.cpp \
    findskiplistdialog.cpp \
    recondialog.cpp \
    PreProcModuleConfigurator.cpp \
    stdafx.cpp \
    viewgeometrylistdialog.cpp \
    preferencesdialog.cpp \
    dialogtoobig.cpp \
    piercingpointdialog.cpp \
    referencefiledlg.cpp \
    globalsettingsdialog.cpp \
    fileconversiondialog.cpp

HEADERS  += muhrecmainwindow.h \
    MuhrecInteractor.h \
    configuregeometrydialog.h \
    findskiplistdialog.h \
    moduleconfigprogressdialog.h \
    recondialog.h \
    PreProcModuleConfigurator.h \
    stdafx.h \
    viewgeometrylistdialog.h \
    preferencesdialog.h \
    dialogtoobig.h \
    piercingpointdialog.h \
    referencefiledlg.h \
    globalsettingsdialog.h \
    fileconversiondialog.h

FORMS    += muhrecmainwindow.ui \
    configuregeometrydialog.ui \
    findskiplistdialog.ui \
    moduleconfigprogressdialog.ui \
    recondialog.ui \
    viewgeometrylistdialog.ui \
    preferencesdialog.ui \
    dialogtoobig.ui \
    piercingpointdialog.ui \
    referencefiledlg.ui \
    globalsettingsdialog.ui \
    fileconversiondialog.ui

CONFIG(release, debug|release):    LIBS += -L$$PWD/../../../../lib/
else:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../lib/debug

LIBS += -lkipl -lModuleConfig -lReconFramework -lQtAddons -lQtModuleConfigure -lImagingAlgorithms -lReaderConfig -lQtImaging -lReaderGUI

INCLUDEPATH += $$PWD/../../../core/kipl/kipl/include
DEPENDPATH  += $$PWD/../../../core/kipl/kipl/include

INCLUDEPATH += $$PWD/../../../GUI/qt/QtModuleConfigure
DEPENDPATH  += $$PWD/../../../GUI/qt/QtModuleConfigure

INCLUDEPATH += $$PWD/../../../GUI/qt/QtAddons
DEPENDPATH  += $$PWD/../../../GUI/qt/QtAddons

INCLUDEPATH += $$PWD/../../../GUI/qt/QtImaging
DEPENDPATH  += $$PWD/../../../GUI/qt/QtImaging

INCLUDEPATH += $$PWD/../../../frameworks/tomography/Framework/ReconFramework/include
DEPENDPATH  += $$PWD/../../../frameworks/tomography/Framework/ReconFramework/src

INCLUDEPATH += $$PWD/../../../core/modules/ModuleConfig/include
DEPENDPATH  += $$PWD/../../../core/modules/ModuleConfig/include

INCLUDEPATH += $$PWD/../../../core/modules/ReaderConfig
DEPENDPATH  += $$PWD/../../../core/modules/ReaderConfig

INCLUDEPATH += $$PWD/../../../core/modules/ReaderGUI
DEPENDPATH  += $$PWD/../../../core/modules/ReaderGUI

INCLUDEPATH += $$PWD/../../../core/algorithms/ImagingAlgorithms/include
DEPENDPATH  += $$PWD/../../../core/algorithms/ImagingAlgorithms/src


macx: {
INCLUDEPATH += $$PWD/../../../external/mac/include
DEPENDPATH += $$PWD/../../../external/mac/include
LIBS += -L$$PWD/../../../external/mac/lib/ -lNeXus.1.0.0 -lNeXusCPP.1.0.0
}

DISTFILES += \
    ../Resources/defaults_linux.xml \
    ../Resources/defaults_mac.xml \
    ../Resources/defaults_windows.xml
