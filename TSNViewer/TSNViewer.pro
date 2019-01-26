#-------------------------------------------------
#
# Project created by QtCreator 2016-10-26T16:24:41
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = TSNViewer
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    tsn.cpp \
    spectrumcal.cpp \
    qcustomplot.cpp \
    clb.cpp \
    clc.cpp \
    tbl.cpp \
    intercal.cpp \
    tsread.cpp \
    tblread.cpp \
    fftw_parameter.cpp \
    yupreader.cpp \
    displays/statisticsspecturmdisplay.cpp \
    files/yup.cpp

HEADERS  += widget.h \
    tsn.h \
    spectrumcal.h \
    qcustomplot.h \
    clb.h \
    clc.h \
    tbl.h \
    intercal.h \
    tsread.h \
    tblread.h \
    fftw3.h \
    fftw_parameter.h \
    yupreader.h \
    displays/statisticsspecturmdisplay.h \
    files/info.h \
    files/yup.h

FORMS    += widget.ui \
    fftw_parameter.ui \
    yupreader.ui \
    displays/statisticsspecturmdisplay.ui

DISTFILES +=

RESOURCES += \
    tsnviewer.qrc

LIBS += \
    C:\Qt_Static\fftw-3.3.5-dll64\libfftw3-3.lib \
    C:\Qt_Static\fftw-3.3.5-dll64\libfftw3f-3.lib \
    C:\Qt_Static\fftw-3.3.5-dll64\libfftw3l-3.lib

