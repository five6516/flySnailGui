#ifndef CMANAGEPLUGIN_H
#define CMANAGEPLUGIN_H
#include <QString>
#include <QLibrary>
#include "Plugin.h"

class CPluginSpec
{
public:
    CPluginSpec();

    bool isPlugin(const QString& fileName);
    bool loadPlugin();
    void releasePlugin();
    QWidget* pluginWidget();
    const char* pluginName();
    const char* pluginContent();
    Plugin_Type pluginType();
    void pluginSetInterFace();
    const char* pluginVersion();
    const char* pluginDescription();
    const char* pluginLicense();

    void setIndex(int index);
    int getIndex();

    void setEnable(bool enable);
    bool getEnable();
private:
    int m_index;
    QLibrary m_Lib;

    using f_initializePlugin = int(*)(int argc, char *argv[]);
    using f_delayedInitialize = int(*)();
    using f_shutdownPlugin = int(*)();
    using f_pluginWidget = QWidget*(*)();
    using f_pluginCategory = const char *(*)();
    using f_pluginName = const char *(*)();
    using f_pluginType = Plugin_Type(*)();
    using f_pluginVersion = const char *(*)();
    using f_pluginDescription = const char *(*)();
    using f_pluginLicense = const char *(*)();

    f_initializePlugin p_initializePlugin = nullptr;
    f_delayedInitialize p_delayedInitialize = nullptr;
    f_shutdownPlugin p_shutdownPlugin = nullptr;
    f_pluginWidget p_pluginWidget = nullptr;
    f_pluginCategory p_pluginCategory = nullptr;
    f_pluginName p_pluginName = nullptr;
    f_pluginType p_pluginType = nullptr;
    f_pluginVersion p_pluginVersion = nullptr;
    f_pluginDescription p_pluginDescription = nullptr;
    f_pluginLicense p_pluginLicense = nullptr;

    bool m_bEnable;
};

#endif // CMANAGEPLUGIN_H
