#include "CorePluginExport.h"
#include "ui/MainWindow.h"
#include "Global.h"

extern MainWindow* w;
bool ShowLogFunc(const QString& data)
{
    if(w == nullptr)
        return  false;

    return w->showLog(data);
}

bool ShowLogPlusFunc(const QString& data, int R,int G,int B,int Size)
{
    if(w == nullptr)
        return  false;

    return w->showLogPlus(data, R,G,B,Size);
}

QString GetGlobalValue(const QString& key)
{
    if(w == nullptr)
        return  "";

    return w->GetGlobalValue(key);
}

void SetGlobalValue(const QString& key,const QString& value)
{
    if(w == nullptr)
        return;

    w->SetGlobalValue(key,value);
}

void addTreeSwitchCallBack(std::function<void(const QString& value)> func)
{
    w->addTreeCallBack(func);
}

void addMenuSwitchCallBack(std::function<void(const QString& value)> func)
{
    w->addMenuCallBack(func);
}

QWidget *getQWidget()
{
    return (QWidget*)w;
}

QString currentTree()
{
    return w->currentTree();
}

QString currentMenu()
{
    return w->currentMenu();
}

void reloadConfig()
{
    w->InitMachineConfig();
}
