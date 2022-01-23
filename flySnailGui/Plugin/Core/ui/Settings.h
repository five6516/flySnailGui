#ifndef SETTINGS_H
#define SETTINGS_H

#include <QApplication>
#include <QSettings>

const QSettings ini(qApp->applicationDirPath()+"/config/ui.ini", QSettings::Format::IniFormat);

#define PluginTOP 0
#define PluginLEFT 1

bool g_ShowLogWindow = ini.value("ui/ShowLogWindow",true).toBool();
bool g_ShowTreeList = ini.value("ui/ShowTreeList",true).toBool();
bool g_ShowTab = ini.value("ui/ShowTab",true).toBool();
bool g_PluginTorL = ini.value("ui/PluginTorL",PluginLEFT).toInt();




#endif // SETTINGS_H
