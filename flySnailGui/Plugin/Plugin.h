#ifndef QT_PLUGIN_H
#define QT_PLUGIN_H
#include <QtCore/qglobal.h>
#include <QWidget>
#include <functional>

#if defined(QT_PLUGIN_LIBRARY)
#  define QT_PLUGIN_EXPORT Q_DECL_EXPORT
#else
#  define QT_PLUGIN_EXPORT Q_DECL_IMPORT
#endif

typedef enum{
    core = 0,
    normal,
    pop,
    server

}Plugin_Type;

//插件过程  initialize->delayedInitialize->using->shutdown

extern "C" QT_PLUGIN_EXPORT int initializePlugin(int argc, char *argv[]);
extern "C" QT_PLUGIN_EXPORT int delayedInitialize();
extern "C" QT_PLUGIN_EXPORT int shutdownPlugin();

//导出一些基础信息
extern "C" QT_PLUGIN_EXPORT QWidget* pluginWidget();//初始化界面
extern "C" QT_PLUGIN_EXPORT const char *pluginCategory();
extern "C" QT_PLUGIN_EXPORT const char *pluginName();
extern "C" QT_PLUGIN_EXPORT Plugin_Type pluginType();//插件类型
extern "C" QT_PLUGIN_EXPORT const char *pluginVersion();
extern "C" QT_PLUGIN_EXPORT const char *pluginDescription();
extern "C" QT_PLUGIN_EXPORT const char *pluginLicense();

#endif // QT_PL_PYSCRIPT_H
