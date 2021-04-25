#ifndef PLUGINMANAGE_H
#define PLUGINMANAGE_H

#include <QString>
#include <map>
#include <string>
#include "PluginSpec.h"
#include "ManageForm.h"

class CPluginManage
{
public:
    explicit CPluginManage(QString path);
    CPluginManage(const CPluginManage&) = delete ;
    CPluginManage& operator=(const CPluginManage&) = delete ;
    ~CPluginManage();

    int findPlugin();
    int loadPlugin();
    void showManageForm();
    void writeSetting();
    void readSetting();

    std::map<std::string,CPluginSpec*> m_mapPlugin;//插件管理类
    QString m_strPluginDir;
    QString m_strSettingFile="/Config/Plugin.ini";
private:
    ManageForm* pManageForm;

    QStringList disabledPlugins;
    QStringList forceEnabledPlugins;

};

#endif // PLUGINMANAGE_H
