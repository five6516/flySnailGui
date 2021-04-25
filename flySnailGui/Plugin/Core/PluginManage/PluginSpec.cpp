#include "PluginSpec.h"
#include "EasyTempLibrarySearchPath.h"
#include "Global.h"
#include <QDebug>

CPluginSpec::CPluginSpec()
{
    m_bEnable = true;
}

void CPluginSpec::setIndex(int index)
{
    m_index = index;
}

int CPluginSpec::getIndex()
{
    return m_index;
}

void CPluginSpec::setEnable(bool enable)
{
    m_bEnable = enable;
}

bool CPluginSpec::getEnable()
{
    return m_bEnable;
}

bool CPluginSpec::isPlugin(const QString& fileName)
{
    if(QLibrary::isLibrary(fileName))
    {
        //确认是否是动态库
        m_Lib.setFileName(fileName);
        if(m_Lib.load())//加载库成功
        {
            p_initializePlugin = (f_initializePlugin)m_Lib.resolve("initializePlugin");
            p_delayedInitialize = (f_delayedInitialize)m_Lib.resolve("delayedInitialize");
            p_shutdownPlugin = (f_shutdownPlugin)m_Lib.resolve("shutdownPlugin");
            p_pluginWidget = (f_pluginWidget)m_Lib.resolve("pluginWidget");
            p_pluginCategory = (f_pluginCategory)m_Lib.resolve("pluginCategory");
            p_pluginName = (f_pluginName)m_Lib.resolve("pluginName");
            p_pluginType = (f_pluginType)m_Lib.resolve("pluginType");
            p_pluginVersion = (f_pluginVersion)m_Lib.resolve("pluginVersion");
            p_pluginDescription = (f_pluginDescription)m_Lib.resolve("pluginDescription");
            p_pluginLicense = (f_pluginLicense)m_Lib.resolve("pluginLicense");

            if(p_initializePlugin != nullptr &&
               p_delayedInitialize != nullptr &&
               p_shutdownPlugin != nullptr &&
               p_pluginWidget != nullptr &&
               p_pluginCategory != nullptr &&
               p_pluginName != nullptr &&
               p_pluginType != nullptr &&
               p_pluginVersion != nullptr &&
               p_pluginDescription != nullptr &&
               p_pluginLicense != nullptr)
            {
                //不加载自己
                if(strcmp(p_pluginName(),"Core")==0)
                    return false;

                return true;
            }
        }
        else
        {
            qDebug() << "load fail";
        }
    }

    return false;
}

bool CPluginSpec::loadPlugin()
{
    if(p_pluginWidget)
    {
        p_initializePlugin(0,0);
        return true;
    }
    return false;
}

QWidget* CPluginSpec::pluginWidget()
{
    if(p_pluginWidget)
    {
        return p_pluginWidget();
    }
    return nullptr;
}

const char* CPluginSpec::pluginName()
{
    if(p_pluginName)
    {
        return p_pluginName();
    }
    return "null";
}

const char* CPluginSpec::pluginContent()
{
    if(p_pluginCategory)
    {
        return p_pluginCategory();
    }
    return "null";
}

Plugin_Type CPluginSpec::pluginType()
{
    if(p_pluginType)
    {
        return p_pluginType();
    }
    return Plugin_Type::normal;
}

void CPluginSpec::releasePlugin()
{
    if(p_shutdownPlugin)
    {
        p_shutdownPlugin();
    }
    m_Lib.unload();
}

const char* CPluginSpec::pluginVersion()
{
    if(p_pluginVersion)
    {
        return p_pluginVersion();
    }
    return "null";
}

const char* CPluginSpec::pluginDescription()
{
    if(p_pluginDescription)
    {
        return p_pluginDescription();
    }
    return "null";
}

const char* CPluginSpec::pluginLicense()
{
    if(p_pluginLicense)
    {
        return p_pluginLicense();
    }
    return "null";
}
