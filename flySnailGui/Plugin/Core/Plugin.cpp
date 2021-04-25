#include "Plugin.h"
#include "Global.h"
#include "ui/MainWindow.h"

MainWindow* w = nullptr;
int initializePlugin(int argc, char *argv[])
{
    (void)argc;(void)argv;
    w = new MainWindow();
    w->hide();
    return 0;
}

int delayedInitialize()
{
    w->InitMachineConfig();
    w->InitPluginugin();
    w->show();
    return 0;
}

int shutdownPlugin()
{
    w->deleteLater();
    return 0;
}

const char *pluginName()
{
    return "Core";
}

const char *pluginCategory()
{
    return "Core";
}

Plugin_Type pluginType()
{
    return Plugin_Type::core;
}

const char *pluginVersion()
{
    return "1.0.1";
}

const char *pluginDescription()
{
    return "";
}

const char *pluginLicense()
{
    return "";
}

QWidget *pluginWidget()
{
    return w;
}
