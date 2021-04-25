#include "PluginManage.h"
#include "EasyTempLibrarySearchPath.h"
#include <QDir>
#include <QDebug>
#include <QLockFile>
#include <QApplication>
#include <QSettings>
#include "ui/MainWindow.h"

const char C_IGNORED_PLUGINS[] = "Plugins/Ignored";
const char C_FORCEENABLED_PLUGINS[] = "Plugins/ForceEnabled";

extern MainWindow* w;
CPluginManage::CPluginManage(QString path)
{
    m_strPluginDir = path;
}

CPluginManage::~CPluginManage()
{

}

int CPluginManage::findPlugin()
{
    readSetting();

    QDir fileDir(m_strPluginDir);

    CEasyTempLibrarySearchPath easylib(m_strPluginDir.toStdString().c_str());

    fileDir.setFilter(QDir::Files);
    QFileInfoList fileInfoList = fileDir.entryInfoList();
    QStringList fileList;
    qDebug()<<fileInfoList.count();
    int i=0;
    for(int inf = 0; inf < fileInfoList.count(); inf ++)
    {
        QString fileName = fileInfoList.at(inf).absoluteFilePath();
        qDebug()<<fileName;
        CPluginSpec* pCPluginSpec = new CPluginSpec;
        if(pCPluginSpec->isPlugin(fileName))
        {
            qDebug()<<fileName<< "is Plugin";

            //配置
            if (disabledPlugins.contains(QString(pCPluginSpec->pluginName())))
                pCPluginSpec->setEnable(false);
            if (forceEnabledPlugins.contains(QString(pCPluginSpec->pluginName())))
                pCPluginSpec->setEnable(true);

            //不存在相同名字的插件，如果有，则不加载
            if(m_mapPlugin.find(std::string(pCPluginSpec->pluginName())) == m_mapPlugin.end())
            {
                pCPluginSpec->setIndex(i++);
                m_mapPlugin.insert(std::pair<std::string,CPluginSpec*>(std::string(pCPluginSpec->pluginName()),pCPluginSpec));
            }
            else
            {
                qDebug()<<fileName<< "has same name Plugin";
            }
        }
    }

    return  0;
}

int CPluginManage::loadPlugin()
{
    for(auto& plugin : m_mapPlugin)
    {
        if(plugin.second->getEnable())//开启的插件才加载
            plugin.second->loadPlugin();
    }

    return 0;
}

void CPluginManage::showManageForm()
{
    ManageForm* pManageForm = new ManageForm(this,w);
    pManageForm->setAttribute(Qt::WA_DeleteOnClose);
    pManageForm->setWindowModality(Qt::ApplicationModal);
    pManageForm->show();
}

void CPluginManage::writeSetting()
{
    QLockFile lock(qApp->applicationDirPath()+m_strSettingFile+".lck");
    QSettings settings(qApp->applicationDirPath()+m_strSettingFile,QSettings::Format::IniFormat);

    disabledPlugins.clear();
    forceEnabledPlugins.clear();

    for (auto it:m_mapPlugin)
    {
        if (it.second->getEnable())
            forceEnabledPlugins.append(it.first.c_str());
        else
            disabledPlugins.append(it.first.c_str());
    }

    settings.setValue(C_IGNORED_PLUGINS,disabledPlugins);
    settings.setValue(C_FORCEENABLED_PLUGINS,forceEnabledPlugins);
}

void CPluginManage::readSetting()
{
    QLockFile lock(qApp->applicationDirPath()+m_strSettingFile+".lck");
    QSettings settings(qApp->applicationDirPath()+m_strSettingFile,QSettings::Format::IniFormat);
    disabledPlugins= settings.value(C_IGNORED_PLUGINS).toStringList();
    forceEnabledPlugins= settings.value(C_FORCEENABLED_PLUGINS).toStringList();
}
