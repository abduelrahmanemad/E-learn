QT       += core gui
QT       += core gui charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    admin.cpp \
    admindashboard.cpp \
    config.cpp \
    course.cpp \
    dialog.cpp \
    main.cpp \
    person.cpp \
    staff.cpp \
    student.cpp

HEADERS += \
    admin.h \
    admindashboard.h \
    config.h \
    course.h \
    dialog.h \
    person.h \
    staff.h \
    student.h

FORMS += \
    admindashboard.ui \
    dialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc \
    resource.qrc

DISTFILES += \
    img/22.png \
    img/33.png \
    img/44.png \
    img/55.png \
    img/77.png \
    img/Untitled-1.png \
    img/add.png \
    img/admin_inactive.png \
    img/arrow-down-sign-to-navigate.png \
    img/book-with-white-bookmark.png \
    img/book.png \
    img/check.png \
    img/csv-file-format-extension.png \
    img/export.png \
    img/import.png \
    img/log-out.png \
    img/logo1.png \
    img/person-removebg-preview (1).png \
    img/person-removebg-preview (1).png \
    img/person.png \
    img/remove.png \
    img/removed.png \
    img/setting.png \
    img/teacher.png \
    img/undefined - Imgur.png \
    img/up.png
