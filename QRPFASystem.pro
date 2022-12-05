QT       += core gui
QT  +=  printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ablationPage/ablationpage.cpp \
    ablationPage/pfablationcontent.cpp \
    ablationPage/rfablationcontent.cpp \
    ablationPage/rfpuncturecontent.cpp \
    ablationPage/rpfasummarycontent.cpp \
    main.cpp \
    mainwindow.cpp \
    public/PfasWidget.cxx \
    public/pfaspage.cpp \
    toolbar/Keyboard/PfasCharacterKeyboard.cxx \
    toolbar/Keyboard/PfasNumberKeyboard.cxx \
    toolbar/Keyboard/PfasWidgetKeyboard.cxx \
    toolbar/NumericBar/numericbar.cpp \
    toolbar/Pageswitchbar/pageswitchbar.cpp \
    toolbar/Puncturebar/puncturebar.cpp \
    toolbar/RFActionbar/rfactionbar.cpp \
    toolbar/RPFStatusBar/rpfstatusbar.cpp \
    toolbar/RingElectrodebar/ringelectrodebar.cpp \
    toolbar/accuCustomplot/customwidget.cpp \
    toolbar/accuCustomplot/qcustomplot.cpp \
    toolbar/customWidgets/PfasButton.cxx

HEADERS += \
    ablationPage/ablationpage.h \
    ablationPage/pfablationcontent.h \
    ablationPage/rfablationcontent.h \
    ablationPage/rfpuncturecontent.h \
    ablationPage/rpfasummarycontent.h \
    mainwindow.h \
    public/PfasUIItem.h \
    public/PfasWidget.h \
    public/pfaspage.h \
    toolbar/Keyboard/PfasCharacterKeyboard.h \
    toolbar/Keyboard/PfasNumberKeyboard.h \
    toolbar/Keyboard/PfasWidgetKeyboard.h \
    toolbar/NumericBar/numericbar.h \
    toolbar/Pageswitchbar/pageswitchbar.h \
    toolbar/Puncturebar/puncturebar.h \
    toolbar/RFActionbar/rfactionbar.h \
    toolbar/RPFStatusBar/rpfstatusbar.h \
    toolbar/RingElectrodebar/ringelectrodebar.h \
    toolbar/accuCustomplot/customwidget.h \
    toolbar/accuCustomplot/qcustomplot.h \
    toolbar/customWidgets/PfasButton.h

FORMS += \
    ablationPage/ablationpage.ui \
    ablationPage/pfablationcontent.ui \
    ablationPage/rfablationcontent.ui \
    ablationPage/rfpuncturecontent.ui \
    ablationPage/rpfasummarycontent.ui \
    mainwindow.ui \
    toolbar/Keyboard/PfasCharacterKeyboard.ui \
    toolbar/Keyboard/PfasNumberKeyboard.ui \
    toolbar/Keyboard/PfasWidgetKeyboard.ui \
    toolbar/Pageswitchbar/pageswitchbar.ui \
    toolbar/Puncturebar/puncturebar.ui \
    toolbar/RFActionbar/rfactionbar.ui \
    toolbar/RPFStatusBar/rpfstatusbar.ui \
    toolbar/RingElectrodebar/ringelectrodebar.ui \
    toolbar/accuCustomplot/customwidget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res/viewer.qrc

DISTFILES +=


