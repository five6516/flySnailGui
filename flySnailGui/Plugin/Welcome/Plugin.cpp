﻿#include "Plugin.h"
#include "Global.h"
#include "Form.h"

Form* form = nullptr;
QWidget *pluginWidget()
{
    return form;
}

int initializePlugin(int argc, char *argv[])
{
    (void)argc;(void)argv;
    form = new Form();
    return 0;
}

int delayedInitialize()
{
    return 0;
}

int shutdownPlugin()
{
    form->deleteLater();
    return 0;
}

const char *pluginName()
{
    return "a首页";
}

const char *pluginCategory()
{
    return "Welcome";
}

Plugin_Type pluginType()
{
    return Plugin_Type::normal;
}

const char *pluginVersion()
{
    return "0.0.0";
}

const char *pluginDescription()
{
    return "";
}

const char *pluginLicense()
{
    return "";
}
