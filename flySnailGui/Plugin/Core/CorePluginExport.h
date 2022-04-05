#ifndef _CORE_PLUGIN_EXPORT_H
#define _CORE_PLUGIN_EXPORT_H

#include <QString>
#include <QWidget>
#include <functional>
#if defined(QT_PLUGIN_LIBRARY)
#  define QT_PLUGIN_EXPORT Q_DECL_EXPORT
#else
#  define QT_PLUGIN_EXPORT Q_DECL_IMPORT
#endif

/* Func */
QT_PLUGIN_EXPORT bool ClearLogFunc();
QT_PLUGIN_EXPORT bool ShowLogFunc(const QString& data);
QT_PLUGIN_EXPORT bool ShowLogPlusFunc(const QString& data, int R,int G,int B,int Size);
QT_PLUGIN_EXPORT void reloadConfig();

/* Data */
QT_PLUGIN_EXPORT QString GetGlobalValue(const QString& key);
QT_PLUGIN_EXPORT void SetGlobalValue(const QString& key,const QString& value);

/* INFO */
QT_PLUGIN_EXPORT QWidget* getQWidget();
QT_PLUGIN_EXPORT QString currentTree();
QT_PLUGIN_EXPORT QString currentMenu();

/* CallBack */
QT_PLUGIN_EXPORT void addTreeSwitchCallBack(std::function<void(const QString& value)> callbackFunc);
QT_PLUGIN_EXPORT void addMenuSwitchCallBack(std::function<void(const QString& value)> callbackFunc);

#endif // QT_PL_PYSCRIPT_H
