TEMPLATE = app
TARGET = delia_gui
INCLUDEPATH += .
INCLUDEPATH += delia_common/include
INCLUDEPATH += thirdparty/AudioFile
QT += gui widgets

# You can make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# Please consult the documentation of the deprecated API in order to know
# how to port your code away from it.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Get the current Git commit hash
DEFINES += MONIQUE_GUI_GIT_COMMIT_HASH="\\\"$(shell git log -1 --format=%H)\\\""

# Paths
INCLUDEPATH += include
INCLUDEPATH += src
INCLUDEPATH += src/widgets

# Input
HEADERS += src/main_window.h
HEADERS += src/gui_msg_thread.h
HEADERS += src/timer.h
HEADERS += src/gui_common.h
HEADERS += src/sound_scope_msg_thread.h
HEADERS += src/utils.h
HEADERS += src/widgets/background.h
HEADERS += src/widgets/bottom_bar.h
HEADERS += src/widgets/chart.h
HEADERS += src/widgets/vcf_cutoff_chart.h
HEADERS += src/widgets/edit_name.h
HEADERS += src/widgets/eg_chart.h
HEADERS += src/widgets/main_list.h
HEADERS += src/widgets/moniq_label.h
HEADERS += src/widgets/msg_box.h
HEADERS += src/widgets/msg_popup.h
HEADERS += src/widgets/multi_status_bar.h
HEADERS += src/widgets/param_value_bar.h
HEADERS += src/widgets/scope.h
HEADERS += src/widgets/status_bar.h
HEADERS += src/widgets/sound_scope.h
HEADERS += src/widgets/wt_file.h
HEADERS += src/widgets/wt_scope.h
HEADERS += include/version.h
SOURCES += src/main.cpp
SOURCES += src/main_window.cpp
SOURCES += src/gui_msg_thread.cpp
SOURCES += src/timer.cpp
SOURCES += src/sound_scope_msg_thread.cpp
SOURCES += src/utils.cpp
SOURCES += src/widgets/background.cpp
SOURCES += src/widgets/bottom_bar.cpp
SOURCES += src/widgets/chart.cpp
SOURCES += src/widgets/vcf_cutoff_chart.cpp
SOURCES += src/widgets/edit_name.cpp
SOURCES += src/widgets/eg_chart.cpp
SOURCES += src/widgets/main_list.cpp
SOURCES += src/widgets/moniq_label.cpp
SOURCES += src/widgets/msg_box.cpp
SOURCES += src/widgets/msg_popup.cpp
SOURCES += src/widgets/multi_status_bar.cpp
SOURCES += src/widgets/param_value_bar.cpp
SOURCES += src/widgets/scope.cpp
SOURCES += src/widgets/sound_scope.cpp
SOURCES += src/widgets/status_bar.cpp
SOURCES += src/widgets/wt_file.cpp
SOURCES += src/widgets/wt_scope.cpp
LIBS += -lrt
RESOURCES = monique_gui.qrc
QMAKE_RESOURCE_FLAGS += -no-compress

CONFIG += c++14 c++17 warn_off

# Set the build folder
CONFIG(debug, debug|release) {
    DESTDIR = build/debug
}
CONFIG(release, debug|release) {
    DESTDIR = build/release
}
OBJECTS_DIR = $$DESTDIR/.obj
MOC_DIR = $$DESTDIR/.moc
RCC_DIR = $$DESTDIR/.qrc
UI_DIR = $$DESTDIR/.u
