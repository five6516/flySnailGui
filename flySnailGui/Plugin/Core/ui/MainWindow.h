#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <memory> // shared_ptr
#include <vector>
#include <thread>
#include <string>
#include <functional>
#include <map>
#include "qtmaterialdialog.h"
#include "qtmaterialappbar.h"
#include "qtmaterialiconbutton.h"
#include "qtmaterialflatbutton.h"
#include "lib/qtmaterialtheme.h"
#include "qtmaterialdrawer.h"
#include "framelessDialog.h"
#include <QApplication>
#include <QMainWindow>
#include "ui/WidgetsWindow.h"
#include "PluginManage/PluginManage.h"
#ifdef Q_OS_WIN
#include "Windows.h"
#endif
class QMainWindow;
class QLibrary;
class QLabel;
class QPushButton;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    bool showLogPlus(const QString& data, int R,int G,int B,int Size);
    bool showLog(const QString& data);
    QString GetGlobalValue(const QString& key);
    void SetGlobalValue(const QString& key,const QString& value);
    void SetStatus(const QString& text);

    int InitPluginugin();
    int InitMachineConfig();
    void addTreeCallBack(std::function<void(const QString& value)> callbackFunc);
    void addMenuCallBack(std::function<void(const QString& value)> callbackFunc);
    QString currentTree();
    QString currentMenu();

private:
    WidgetsWindow* m_pWidgetsWindow;

    /******* 导航栏 *******/
    QtMaterialAppBar* m_pQtMaterialAppBar;

    /******* 状态栏 *******/
    QLabel* m_statusLabel;

    /******* 工具栏按钮 *******/
    QtMaterialIconButton *infoAct;
    QtMaterialIconButton *manageAct;
    QtMaterialIconButton *treeAct;
    QtMaterialIconButton *clearAct;
    QtMaterialIconButton *saveAct;
    QtMaterialIconButton *closeAct;
    QtMaterialIconButton *maxAct;
    QtMaterialIconButton *minAct;

    void setNormalRect();
    void addTotalUI();
    void addToolUI();
    void addStatusUI();

    std::map<QString,QString> m_globalMap;
    CPluginManage* m_pCPluginManage = nullptr;
protected:
    /******* 界面美化 *******/
    void paintEvent(QPaintEvent *);
    bool eventFilter(QObject *watched, QEvent *event);
    int padding = 8;
    QRect normalRect;
    QRect rectLeft;
    QRect rectTop;
    QRect rectRight;
    QRect rectBottom;
    QRect rectLeftTop;
    QRect rectRightTop;
    QRect rectLeftBottom;
    QRect rectRightBottom;
    int rectX;
    int rectY;
    int rectW;
    int rectH;
    bool onepressed = false;
    bool pressed = false;
    bool pressedLeft = false;
    bool pressedRight = false;
    bool pressedTop = false;
    bool pressedBottom = false;
    bool pressedLeftTop = false;
    bool pressedRightTop = false;
    bool pressedLeftBottom = false;
    bool pressedRightBottom = false;

    /* 记录鼠标按下的坐标位置 */
    QPoint m_currentPoint;
    QPoint m_pressPoint;
    bool m_bResizeEnable = true;
    bool m_bMoveEnable = true;
    bool  m_isMax = false;
    /*********************/

public slots:
    void buttonVersion(bool);
    void buttonSaveLog(bool);
    void buttonClearLog(bool);
    void buttonClose(bool);
    void buttonMax(bool);
    void buttonMin(bool);
    void buttonTree(bool);
};

#endif // MAINWINDOW_H
