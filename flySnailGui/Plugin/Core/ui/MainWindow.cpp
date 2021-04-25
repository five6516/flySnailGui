#include "MainWindow.h"
#include "Plugin.h"
#include <QDir>
#include <QDebug>
#include <QLabel>
#include <QPainter>
#include <QMouseEvent>
#include <QDesktopWidget>
#include <QLabel>
#include <QApplication>
#include <QMainWindow>
#include <QLockFile>
#include <QTreeWidgetItem>
#include <QTreeWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLibrary>
#include <QMenu>
#include <QTextCodec>
#include <QStatusBar>
#include <QDockWidget>
#include <QSystemTrayIcon>
#include "Global.h"
#include "qtmaterialdialog.h"
#include <QTimer>
#include "WidgetsWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setWindowTitle("flysnailGui");

    /* 设置窗口大小 */
    setNormalRect();

    /* 设置无标题栏和无边框窗口 */
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
    //setAttribute(Qt::WA_TranslucentBackground);
    setAutoFillBackground(true);

    setAttribute(Qt::WA_Hover,true);//开启悬停事件

    addTotalUI();

    installEventFilter(this);

    buttonMax(true);
}

void MainWindow::addTotalUI()
{
    /* 状态栏 */
    addStatusUI();

    /* 工具栏 */
    addToolUI();

    m_pCPluginManage = new CPluginManage(qApp->applicationDirPath() + "/Plugin");
    m_pWidgetsWindow = new WidgetsWindow(m_pCPluginManage);

    QVBoxLayout *pTotalVBoxLayout = new QVBoxLayout();
    pTotalVBoxLayout->setSpacing(0);
    pTotalVBoxLayout->setMargin(0);
    pTotalVBoxLayout->addWidget(m_pQtMaterialAppBar);

    pTotalVBoxLayout->addWidget(m_pWidgetsWindow);
    pTotalVBoxLayout->addWidget(m_statusLabel);


    QWidget* centerWidget = new QWidget();
    setCentralWidget(centerWidget);
    centerWidget->setLayout(pTotalVBoxLayout);
}


void MainWindow::addStatusUI()
{
    m_statusLabel = new QLabel(pluginVersion());
    m_statusLabel->setAlignment(Qt::AlignRight);
    m_statusLabel->setMargin(5);
    m_statusLabel->setAttribute(Qt::WA_TranslucentBackground);
    m_statusLabel->setForegroundRole(QPalette::Foreground);
    m_statusLabel->setFont(QFont("Roboto", 11, QFont::Normal));
}

void MainWindow::addToolUI()
{
    manageAct = new QtMaterialIconButton(QtMaterialTheme::icon("action", "view_stream"));
    infoAct = new QtMaterialIconButton(QtMaterialTheme::icon("action", "info"));
    treeAct = new QtMaterialIconButton(QtMaterialTheme::icon("navigation", "menu"));
    clearAct = new QtMaterialIconButton(QtMaterialTheme::icon("content","delete_sweep"));
    saveAct = new QtMaterialIconButton(QtMaterialTheme::icon("content","save"));
    closeAct = new QtMaterialIconButton(QtMaterialTheme::icon("navigation","close"));
    maxAct = new QtMaterialIconButton(QtMaterialTheme::icon("navigation","fullscreen"));
    minAct = new QtMaterialIconButton(QtMaterialTheme::icon("content","remove"));

    manageAct->setToolTip("插件管理");
    infoAct->setToolTip("说明");
    treeAct->setToolTip("展开插件列表");
    clearAct->setToolTip("清空log");
    saveAct->setToolTip("保存log");
    connect(manageAct, &QtMaterialIconButton::clicked,this,[&](bool){
        m_pCPluginManage->showManageForm();
    });
    connect(infoAct, SIGNAL(clicked(bool)),this,SLOT(buttonVersion(bool)));
    connect(clearAct, SIGNAL(clicked(bool)),this,SLOT(buttonClearLog(bool)));
    connect(saveAct, SIGNAL(clicked(bool)),this,SLOT(buttonSaveLog(bool)));
    connect(closeAct, SIGNAL(clicked(bool)),this,SLOT(buttonClose(bool)));
    connect(maxAct, SIGNAL(clicked(bool)),this,SLOT(buttonMax(bool)));
    connect(minAct, SIGNAL(clicked(bool)),this,SLOT(buttonMin(bool)));
    connect(treeAct, SIGNAL(clicked(bool)),this,SLOT(buttonTree(bool)));

    /* 工具栏 */
    m_pQtMaterialAppBar = new QtMaterialAppBar();
    m_pQtMaterialAppBar->setBackgroundColor(QColor("#4dd0e1"));
    m_pQtMaterialAppBar->setMaximumHeight(35);

    QHBoxLayout *pQHBoxLayout_AppMenu = new QHBoxLayout();
    pQHBoxLayout_AppMenu->addWidget(treeAct);
    pQHBoxLayout_AppMenu->addWidget(clearAct);
    pQHBoxLayout_AppMenu->addWidget(saveAct);
    pQHBoxLayout_AppMenu->addWidget(infoAct);
    pQHBoxLayout_AppMenu->addWidget(manageAct);

    m_pQtMaterialAppBar->appBarLayout()->setSpacing(10);
    m_pQtMaterialAppBar->appBarLayout()->addLayout(pQHBoxLayout_AppMenu);
    m_pQtMaterialAppBar->appBarLayout()->addStretch(1);
    m_pQtMaterialAppBar->appBarLayout()->addWidget(new QLabel("core: "+QString(pluginVersion()) + "  "));
    m_pQtMaterialAppBar->appBarLayout()->addWidget(minAct);
    m_pQtMaterialAppBar->appBarLayout()->addWidget(maxAct);
    m_pQtMaterialAppBar->appBarLayout()->addWidget(closeAct);
}


MainWindow::~MainWindow()
{
    if(m_pCPluginManage)
        delete m_pCPluginManage;
}

int MainWindow::InitPluginugin()
{  
    m_pWidgetsWindow->loadSMVTPlugin();
    return  0;
}

int MainWindow::InitMachineConfig()
{
    m_pWidgetsWindow->loadSMVTConfig();
    return  0;
}

void MainWindow::addTreeCallBack(std::function<void(const QString& value)> func)
{
    m_pWidgetsWindow->addTreeCallBack(func);
}

void MainWindow::addMenuCallBack(std::function<void(const QString& value)> func)
{
    m_pWidgetsWindow->addMenuCallBack(func);
}

QString MainWindow::currentTree()
{
    return  m_pWidgetsWindow->currentTree();
}

QString MainWindow::currentMenu()
{
    return  m_pWidgetsWindow->currentMenu();
}

void MainWindow::setNormalRect()
{
    QDesktopWidget* desk = QApplication::desktop();
    int width = desk->width()*2/3;
    int height = desk->height()*2/3;
    int ax = desk->width()/2 - width/2;
    int ay = desk->height()/2 - height/2;
    normalRect = QRect(ax, ay, width, height);
    setGeometry(normalRect);
}

bool MainWindow::showLogPlus(const QString& data, int R,int G,int B,int Size)
{
    QMetaObject::invokeMethod(m_pWidgetsWindow,"showLogPlusSlot",Qt :: AutoConnection,
                              Q_ARG(QString,data),
                              Q_ARG(int,R),
                              Q_ARG(int,G),
                              Q_ARG(int,B),
                              Q_ARG(int,Size));
    return true;
}

bool MainWindow::showLog(const QString& data)
{
    QMetaObject::invokeMethod(m_pWidgetsWindow,"showLogSlot",Qt :: AutoConnection,Q_ARG(QString,data));
    return true;
}

QString MainWindow::GetGlobalValue(const QString &key)
{
    if(m_globalMap.find(key) != m_globalMap.end())
        return  m_globalMap.find(key)->second;
    return "";
}

void MainWindow::SetGlobalValue(const QString &key, const QString &value)
{
}

void MainWindow::buttonVersion(bool checked)
{ 
    (void)checked;
}

void MainWindow::buttonSaveLog(bool)
{
    QMetaObject::invokeMethod(m_pWidgetsWindow,"buttonSaveLog",Qt :: AutoConnection);
}

void MainWindow::buttonClearLog(bool)
{
    QMetaObject::invokeMethod(m_pWidgetsWindow,"buttonClearLog",Qt :: AutoConnection);
}

void MainWindow::buttonClose(bool)
{
    close();
}

void MainWindow::buttonMax(bool)
{
    if(m_isMax)
    {
        setGeometry(normalRect);
        maxAct->setIcon(QtMaterialTheme::icon("navigation","fullscreen"));
        m_isMax = false;
    }
    else
    {
        maxAct->setIcon(QtMaterialTheme::icon("navigation","fullscreen_exit"));
        QDesktopWidget* desk = QApplication::desktop();
        int width = desk->width();
        int height = desk->height();
        int ax = desk->width()/2 - width/2;
        int ay = desk->height()/2 - height/2;
        setGeometry(QRect(ax, ay, width, height));
        m_isMax = true;
    }
}

void MainWindow::buttonMin(bool)
{
    showMinimized();
}

void MainWindow::buttonTree(bool)
{
    m_pWidgetsWindow->showTreeWidget();
}

void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QPen pen(QColor("WhiteSmoke"));
    pen.setWidth(1);
    painter.setPen(pen);
    painter.setBrush(QBrush(QColor("WhiteSmoke")));
    painter.drawRoundedRect(m_pQtMaterialAppBar->x(), m_pQtMaterialAppBar->y(), width()-m_pQtMaterialAppBar->x()*2, height()-m_pQtMaterialAppBar->y()*2, 5, 5);
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    QWidget* widget = (QWidget*)watched;

    QMouseEvent *mouseEvent = (QMouseEvent *)event;
    m_currentPoint = mouseEvent->pos();

    switch(event->type())
    {
    case QEvent::Resize:
    {
        //重新计算八个描点的区域,描点区域的作用还有就是计算鼠标坐标是否在某一个区域内
        int width = widget->width();
        int height = widget->height();
        rectLeft = QRect(0, padding, padding, height - padding * 2);//左侧描点区域
        rectTop = QRect(padding, 0, width - padding * 2, padding);//上侧描点区域
        rectRight = QRect(width - padding, padding, padding, height - padding * 2);//右侧描点区域
        rectBottom = QRect(padding, height - padding, width - padding * 2, padding);//下侧描点区域
        rectLeftTop = QRect(0, 0, padding, padding);//左上角描点区域
        rectRightTop = QRect(width - padding, 0, padding, padding);//右上角描点区域
        rectLeftBottom = QRect(0, height - padding, padding, padding);//左下角描点区域
        rectRightBottom = QRect(width - padding, height - padding, padding, padding);//右下角描点区域
    }
    break;
    case QEvent::HoverMove:
    {
        //设置对应鼠标形状,这个必须放在这里而不是下面,因为可以在鼠标没有按下的时候识别
        QHoverEvent *hoverEvent = (QHoverEvent *)event;
        QPoint point = hoverEvent->pos();
        if (m_bResizeEnable && !onepressed && !m_isMax)
        {
            if (rectLeft.contains(point)) {
                widget->setCursor(Qt::SizeHorCursor);
            } else if (rectRight.contains(point)) {
                widget->setCursor(Qt::SizeHorCursor);
            } else if (rectTop.contains(point)) {
                widget->setCursor(Qt::SizeVerCursor);
            } else if (rectBottom.contains(point)) {
                widget->setCursor(Qt::SizeVerCursor);
            } else if (rectLeftTop.contains(point)) {
                widget->setCursor(Qt::SizeFDiagCursor);
            } else if (rectRightTop.contains(point)) {
                widget->setCursor(Qt::SizeBDiagCursor);
            } else if (rectLeftBottom.contains(point)) {
                widget->setCursor(Qt::SizeBDiagCursor);
            } else if (rectRightBottom.contains(point)) {
                widget->setCursor(Qt::SizeFDiagCursor);
            } else {
                widget->setCursor(Qt::ArrowCursor);
            }
        }

        //根据当前鼠标位置,计算XY轴移动了多少
        int offsetX = point.x() - m_pressPoint.x();
        int offsetY = point.y() - m_pressPoint.y();

        //根据按下处的位置判断是否是移动控件还是拉伸控件
        if (m_bMoveEnable && pressed) {
            if(m_isMax)
            {
                setGeometry(point.x()-normalRect.width()/2,point.y(),normalRect.width(),normalRect.height());
                //pressed = false;
                m_pressPoint = m_currentPoint-QPoint(-normalRect.width()/2,0);
                maxAct->setIcon(QtMaterialTheme::icon("navigation","fullscreen"));
                m_isMax = false;
            }
            else
            {
                this->move(widget->x() + offsetX, widget->y() + offsetY);
            }
        }
        else if (m_bResizeEnable && onepressed && !m_isMax) {
            if (pressedLeft) {
                int resizeW = widget->width() - offsetX;
                if (widget->minimumWidth() <= resizeW) {
                    widget->setGeometry(widget->x() + offsetX, rectY, resizeW, rectH);
                }
            } else if (pressedRight) {
                widget->setGeometry(rectX, rectY, rectW + offsetX, rectH);
            } else if (pressedTop) {
                int resizeH = widget->height() - offsetY;
                if (widget->minimumHeight() <= resizeH) {
                    widget->setGeometry(rectX, widget->y() + offsetY, rectW, resizeH);
                }
            } else if (pressedBottom) {
                widget->setGeometry(rectX, rectY, rectW, rectH + offsetY);
            } else if (pressedLeftTop) {
                int resizeW = widget->width() - offsetX;
                int resizeH = widget->height() - offsetY;
                if (widget->minimumWidth() <= resizeW) {
                    widget->setGeometry(widget->x() + offsetX, widget->y(), resizeW, resizeH);
                }
                if (widget->minimumHeight() <= resizeH) {
                    widget->setGeometry(widget->x(), widget->y() + offsetY, resizeW, resizeH);
                }
            } else if (pressedRightTop) {
                int resizeW = rectW + offsetX;
                int resizeH = widget->height() - offsetY;
                if (widget->minimumHeight() <= resizeH) {
                    widget->setGeometry(widget->x(), widget->y() + offsetY, resizeW, resizeH);
                }
            } else if (pressedLeftBottom) {
                int resizeW = widget->width() - offsetX;
                int resizeH = rectH + offsetY;
                if (widget->minimumWidth() <= resizeW) {
                    widget->setGeometry(widget->x() + offsetX, widget->y(), resizeW, resizeH);
                }
                if (widget->minimumHeight() <= resizeH) {
                    widget->setGeometry(widget->x(), widget->y(), resizeW, resizeH);
                }
            } else if (pressedRightBottom) {
                int resizeW = rectW + offsetX;
                int resizeH = rectH + offsetY;
                widget->setGeometry(widget->x(), widget->y(), resizeW, resizeH);
            }
        }
    }
    break;
    case QEvent::MouseButtonPress:
    {
        QMouseEvent *mouseEvent = (QMouseEvent *)event;
        if (mouseEvent->button() == Qt::LeftButton)
        {
            //记住当前控件坐标和宽高以及鼠标按下的坐标
            rectX = widget->x();
            rectY = widget->y();
            rectW = widget->width();
            rectH = widget->height();
            m_pressPoint = mouseEvent->pos();

            //判断按下的手柄的区域位置
            onepressed = true;
            if (rectLeft.contains(m_pressPoint)) {
                pressedLeft = true;
            } else if (rectRight.contains(m_pressPoint)) {
                pressedRight = true;
            } else if (rectTop.contains(m_pressPoint)) {
                pressedTop = true;
            } else if (rectBottom.contains(m_pressPoint)) {
                pressedBottom = true;
            } else if (rectLeftTop.contains(m_pressPoint)) {
                pressedLeftTop = true;
            } else if (rectRightTop.contains(m_pressPoint)) {
                pressedRightTop = true;
            } else if (rectLeftBottom.contains(m_pressPoint)) {
                pressedLeftBottom = true;
            } else if (rectRightBottom.contains(m_pressPoint)) {
                pressedRightBottom = true;
            } else {
                if(m_pQtMaterialAppBar == qobject_cast<QtMaterialAppBar *>(childAt(mouseEvent->pos())))
                    pressed = true;
            }
        }
    }
    break;
    case QEvent::MouseMove:
    {
        //改成用HoverMove识别
    }
    break;
    case QEvent::MouseButtonDblClick:
    {
        QMouseEvent *mouseEvent = (QMouseEvent *)event;
        if (m_pQtMaterialAppBar == qobject_cast<QtMaterialAppBar *>(childAt(mouseEvent->pos())))
        {
            buttonMax(true);
        }
    }
    break;
    case QEvent::MouseButtonRelease:
    {
        //恢复所有
        onepressed = false;
        pressed = false;
        pressedLeft = false;
        pressedRight = false;
        pressedTop = false;
        pressedBottom = false;
        pressedLeftTop = false;
        pressedRightTop = false;
        pressedLeftBottom = false;
        pressedRightBottom = false;
        widget->setCursor(Qt::ArrowCursor);
    }
    break;
    default:
    break;
    }

    return QObject::eventFilter(watched, event);
}
