#include <QApplication>
#include <QStyleFactory>
#include <QFile>
#include <QString>
#include <QLibrary>
#include <QTextStream>
#include <QMessageBox>
#include <QSettings>
#include <QProcess>
#include <QDebug>
#include <QDesktopServices>
#include <QUrl>

#include "httplib.h"
#include <json/json.h>

#if (defined(_MSC_VER))
#pragma execution_character_set("utf-8")
#endif

QString strQSS;
QString strStyleFactory;
QString strFont;
int iFontSize;
QString strColor;

void loadConfig()
{
    QSettings ini(qApp->applicationDirPath()+"/config/ui.ini", QSettings::Format::IniFormat);
    strQSS = ini.value("theme/QSS","qss.css").toString();
    strStyleFactory = ini.value("theme/StyleFactory","Fusion").toString();
    strFont = ini.value("theme/QSS","Font").toString();
    iFontSize = ini.value("theme/FontSize",11).toInt();
    strColor = ini.value("theme/Color","white").toString();
}

QString loadQssFile()
{
    QString qss;
    QFile file(":/" + strQSS);
    if (file.open(QFile::ReadOnly)) {
        QStringList list;
        QTextStream in(&file);
        while (!in.atEnd())
        {
            QString line;
            in >> line;
            list << line;
        }
        qss = list.join("\n");
        file.close();
        return qss;
    }
    return "";
}

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QCoreApplication::setAttribute(Qt::AA_UseStyleSheetPropagationInWidgetStyles, true);
    QApplication a(argc, argv);

    loadConfig();

    a.setStyleSheet(loadQssFile());//加载样式表
    a.setStyle(QStyleFactory::create(strStyleFactory));
    a.setFont(QFont(strFont, iFontSize, QFont::Normal));

    /* 设置背景颜色 */
    QPalette palette = a.palette();
    palette.setColor(QPalette::Background, strColor);
    a.setPalette(palette);

    /* update */
    QSettings ini(qApp->applicationDirPath()+"/config/ui.ini", QSettings::Format::IniFormat);
    if(ini.value("update/open","0").toInt() == 1)
    {
        httplib::Client cli(ini.value("update/url","10.92.35.172").toString().toStdString(), ini.value("update/port","8000").toInt());
        if (auto res = cli.Get(QString::asprintf("%s%s",ini.value("update/version_path","/static/Other/").toString().toStdString().c_str(),ini.value("update/version_name","version.json").toString().toStdString().c_str()).toStdString().c_str()))
        {
            if(res->status == 200)
            {
                Json::Reader reader;
                Json::Value root;
                Json::Reader reader_current;
                Json::Value root_current;

                QFile file(qApp->applicationDirPath()+"/"+ini.value("update/version_name","version.json").toString());
                if(file.open(QIODevice::ReadOnly))
                {
                    if (!reader.parse(res->body, root, false) && !reader_current.parse(QString(file.readAll()).toStdString(), root_current, false))
                    {
                        qDebug() << root[0]["version"].asInt();
                        qDebug() << root[0]["author"].asCString();
                        qDebug() << root[0]["Descrip"].asCString();
                        qDebug() << root_current[0]["version"].asInt();
                        qDebug() << root_current[0]["author"].asCString();
                        qDebug() << root_current[0]["Descrip"].asCString();
                        if(root[0]["version"] > root_current[0]["version"])
                        {
                            QString warningStr = QString::asprintf("有新的下载版本:V%d\n\n当前版本为:V%d\n\n更新内容为:\n\n%s\n\n下载账号:custom\n\n下载密码:custom",
                                    root[0]["version"].asInt(),
                                    root_current[0]["version"].asInt(),
                                    root[0]["Descrip"].asCString());
                            int ret = QMessageBox::warning(NULL, "检查更新",  warningStr, "去下载", "不更新");
                            if(ret == 0)    //点击更新
                            {
                                QDesktopServices::openUrl(QUrl(ini.value("update/download_url","http://192.168.1.1:8000/").toString()));
                                exit(0);
                            }
                        }
                    }
                    file.close();
                }
            }
        }
        else
        {
            qDebug() << "error code: ";
        }
    }

    /* 加载核心库 */
    QString coreFile = qApp->applicationDirPath() + "/Plugin/";

#if (defined(Q_OS_WIN))
    coreFile += "Core.dll";
#elif (defined(Q_OS_LINUX))
    coreFile += "libCore.so";
#endif

    using f_initializePlugin = int (*)(int argc, char *argv[]);
    using f_delayedInitialize = int (*)();
    using f_shutdownPlugin = int (*)();
    using f_pluginName = const char * (*)();
    f_initializePlugin p_initializePlugin = nullptr;
    f_delayedInitialize p_delayedInitialize = nullptr;
    f_shutdownPlugin p_shutdownPlugin = nullptr;
    f_pluginName p_pluginName = nullptr;

    QLibrary m_Lib;
    if(QLibrary::isLibrary(coreFile))
    {
        //确认是否是动态库
        m_Lib.setFileName(coreFile);
        if(m_Lib.load())//加载库成功
        {
            p_initializePlugin = (f_initializePlugin)m_Lib.resolve("initializePlugin");
            p_delayedInitialize = (f_delayedInitialize)m_Lib.resolve("delayedInitialize");
            p_shutdownPlugin = (f_shutdownPlugin)m_Lib.resolve("shutdownPlugin");
            p_pluginName = (f_pluginName)m_Lib.resolve("pluginName");

            if(p_initializePlugin == nullptr ||
               p_shutdownPlugin == nullptr ||
               p_pluginName == nullptr ||
               p_delayedInitialize == nullptr)
            {
                QMessageBox msgBox(QMessageBox::Icon::NoIcon,"Core Plugin加载异常","无法加载核心库");
                msgBox.exec();
                m_Lib.unload();
                return 0;
            }
            if(strcmp(p_pluginName(),"Core") != 0)
            {
                QMessageBox msgBox(QMessageBox::Icon::NoIcon,"Core Plugin加载异常","无法加载核心库");
                msgBox.exec();
                m_Lib.unload();
                return 0;
            }

            p_initializePlugin(argc,argv);
            p_delayedInitialize();
        }
        else
        {
            QMessageBox msgBox(QMessageBox::Icon::NoIcon,"Core Plugin加载异常","无法加载核心库");
            msgBox.exec();
            return 0;
        }
    }

    QObject::connect(&a, &QApplication::aboutToQuit, [&](){
        p_shutdownPlugin();
        m_Lib.unload();
    });

    return a.exec();
}
