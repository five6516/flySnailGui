OUT_DIR = $$PWD/../Bin


macx{
# mac only
}
android{
    PlatForm = Android
}
unix:!macx{
    !android{
        PlatForm = Linux
    }
    LIBS=-ldl
    QMAKE_LFLAGS += -Wl,-z,origin \'-Wl,-rpath,"./"\'
    QMAKE_CXXFLAGS += -fvisibility=hidden
    QMAKE_LFLAGS += -Wl,-Bsymbolic
}
win32 {
    PlatForm = Windows

    QMAKE_CXXFLAGS += /MP
}

CONFIG(debug, debug|release){
    DebugOrRelease = Debug
}else{
    DebugOrRelease = Release
}

contains(QT_ARCH, i386) {
    ARCHITECTURE = x86
} else {
    ARCHITECTURE = x64
}

!android{

OBJECTS_DIR = temp/$${PlatForm}/$${ARCHITECTURE}/$${DebugOrRelease}/obj
MOC_DIR = temp/$${PlatForm}/$${ARCHITECTURE}/$${DebugOrRelease}/moc
RCC_DIR = temp/$${PlatForm}/$${ARCHITECTURE}/$${DebugOrRelease}/rcc
UI_DIR = temp/$${PlatForm}/$${ARCHITECTURE}/$${DebugOrRelease}/ui

contains(DEFINES, Lib_PLUGIN_LIBRARY){
    DESTDIR = $$PWD/lib/$${ARCHITECTURE}/$${DebugOrRelease}
    DLLDESTDIR = $$OUT_DIR/$${PlatForm}/$${ARCHITECTURE}/$${DebugOrRelease}

    #message("Components")

CONFIG(debug, debug|release){#用于调试信息
    CONFIG += file_copies
    components_copy.files += $$PWD/lib/$${ARCHITECTURE}/$${DebugOrRelease}
    components_copy.path += $$OUT_DIR/$${PlatForm}/$${ARCHITECTURE}

    # 配置COPIES
    COPIES += components_copy
    }
}

#插件
contains(DEFINES, Tool_Plugin){
    DLLDESTDIR = $$OUT_DIR/$${PlatForm}/$${ARCHITECTURE}/$${DebugOrRelease}/Plugin
    unix: DESTDIR = $$DLLDESTDIR
    #message("Plugin")
}

#核心插件
contains(DEFINES, Core_Plugin){
    DESTDIR = $$PWD/lib/$${ARCHITECTURE}/$${DebugOrRelease}
    DLLDESTDIR = $$OUT_DIR/$${PlatForm}/$${ARCHITECTURE}/$${DebugOrRelease}/Plugin
    unix: DESTDIR = $$DLLDESTDIR
    #message("Core Plugin")
}

#主程序
contains(DEFINES, Tool_Plugin|Lib_PLUGIN_LIBRARY|Core_Plugin){

} else {
    unix: QMAKE_LFLAGS += -no-pie
    DESTDIR = $$OUT_DIR/$${PlatForm}/$${ARCHITECTURE}/$${DebugOrRelease}
    #message("Main")
}

#message($$OUT_DIR)
#message($$DESTDIR)
}
