INCLUDEPATH += \
    $$PWD \
    $$PWD/../

HEADERS += \
    $$PWD/Global.h \

SOURCES += \

LIBS+= -L$$PWD/../../lib/$${ARCHITECTURE}/$${DebugOrRelease} -lWidgetsLib -lJsonCppLib -lQCustomPlot

INCLUDEPATH+=$$PWD/../../Library/WidgetsLib \
             $$PWD/../../Library/JsonCppLib \
             $$PWD/../../Library/QCustomPlot

DEPENDPATH+=$$PWD/../../Library/WidgetsLib \
            $$PWD/../../Library/JsonCppLib \
            $$PWD/../../Library/QCustomPlot

contains(DEFINES, Tool_Plugin){
    LIBS+= -L$$PWD/../../lib/$${ARCHITECTURE}/$${DebugOrRelease} -lCore -L$$DLLDESTDIR
    INCLUDEPATH+= $$PWD/../../Plugin/Core
    DEPENDPATH+=$$PWD/../../Plugin/Core
}

win32:{
    LIBS +=shell32.lib
    LIBS +=shlwapi.lib
    LIBS +=user32.lib
    LIBS +=comctl32.lib
    LIBS +=imm32.lib
}

#禁用qdebug打印输出
DEFINES     += QT_NO_DEBUG_OUTPUT
