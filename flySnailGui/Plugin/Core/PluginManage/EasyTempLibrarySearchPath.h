#ifndef CEASYTEMPLIBRARYSEARCHPATH_H
#define CEASYTEMPLIBRARYSEARCHPATH_H

#include <string>
#include <QString>
#include <QStringList>
#include <QMutex>
#include <QSemaphore>

class CEasyTempLibrarySearchPath
{
public:
    CEasyTempLibrarySearchPath(const char *strPath)
    {
        m_strPath=strPath;
        lockLibrarySearchPathMutex();
        setLibrarySearchPath(m_strPath.c_str());
    }

    CEasyTempLibrarySearchPath(QString strPath)
    {
        m_strPath=strPath.toStdString();
        lockLibrarySearchPathMutex();
        setLibrarySearchPath(m_strPath.c_str());
    }

    ~CEasyTempLibrarySearchPath()
    {
        removeLibrarySearchPath(m_strPath.c_str());
        unlockLibrarySearchPathMutex();
    }

    void setLibrarySearchPath(QString strPath);
    void removeLibrarySearchPath(QString strPath);
    void lockLibrarySearchPathMutex();
    void unlockLibrarySearchPathMutex();
private:
    std::string m_strPath;
    QMutex m_mutexLibrarySearchPath{QMutex::RecursionMode::Recursive};
};

#endif // CEASYTEMPLIBRARYSEARCHPATH_H
