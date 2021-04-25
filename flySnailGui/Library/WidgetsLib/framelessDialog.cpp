#include "framelessDialog.h"
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPainter>
#include <QMouseEvent>
#include <QApplication>
#include <QDesktopWidget>
#include <QDebug>

framelessDialog::framelessDialog(QWidget *parent):QDialog(parent)
{
    QDesktopWidget* desk = QApplication::desktop();
    int width = desk->width();
    int height = desk->height();
    int ax = desk->width()/2 - width/2;
    int ay = desk->height()/2 - height/2;
    fullScreenRect=QRect(ax,ay,width,height);
    normalRect = geometry();

    qDebug() << normalRect;

    setAttribute(Qt::WA_Hover,true);//开启悬停事件

    /* 设置无标题栏和无边框窗口 */
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setAutoFillBackground(true);

    /* 设置背景颜色 */
    QPalette palette = this->palette();
    palette.setColor(QPalette::Background, "white");
    this->setPalette(palette);

    /* 设置对话框样式表 */
    this->setStyleSheet(R"(
        QDialog {
            border-radius: 5px;
            border-width: 2px;
        }
        )");

    /* 工具栏按钮 */
    closeAct = new QtMaterialIconButton(QtMaterialTheme::icon("navigation","close"));
    maxAct = new QtMaterialIconButton(QtMaterialTheme::icon("navigation","fullscreen"));
    minAct = new QtMaterialIconButton(QtMaterialTheme::icon("content","remove"));
    connect(closeAct, &QAbstractButton::clicked,this,[&](bool clicked){
        close();
    });
    connect(maxAct, &QAbstractButton::clicked,this,[&](bool clicked){
       showMaximized();
    });
    connect(minAct, &QAbstractButton::clicked,this,[&](bool clicked){
       showMinimized();
    });

    /* 工具栏 */
    m_pQtMaterialAppBar = new QtMaterialAppBar();
    m_pQtMaterialAppBar->setMaximumHeight(35);
    m_pQtMaterialAppBar->appBarLayout()->setSpacing(10);
    m_pQHBoxLayoutAppMenu = new QHBoxLayout();
    m_pQtMaterialAppBar->appBarLayout()->addLayout(m_pQHBoxLayoutAppMenu);
    m_pQtMaterialAppBar->appBarLayout()->addStretch(1);
    m_pQtMaterialAppBar->appBarLayout()->addWidget(minAct);
    m_pQtMaterialAppBar->appBarLayout()->addWidget(maxAct);
    m_pQtMaterialAppBar->appBarLayout()->addWidget(closeAct);

    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->setSpacing(0);
    vLayout->setMargin(0);
    vLayout->addWidget(m_pQtMaterialAppBar, 0, Qt::AlignTop);
    m_widgetLayout = new QHBoxLayout;
    vLayout->addLayout(m_widgetLayout);
    setLayout(vLayout);

    installEventFilter(this);
}

QHBoxLayout *framelessDialog::getLayout()
{
    return m_widgetLayout;
}

QHBoxLayout *framelessDialog::getAppBarLayout()
{
    return m_pQHBoxLayoutAppMenu;
}


void framelessDialog::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QPen pen(QColor("WhiteSmoke"));
    pen.setWidth(1);
    painter.setPen(pen);
    painter.setBrush(QBrush(QColor("WhiteSmoke")));
    painter.drawRoundedRect(m_pQtMaterialAppBar->x(), m_pQtMaterialAppBar->y(),
                            width()-m_pQtMaterialAppBar->x()*2, height()-m_pQtMaterialAppBar->y()*2,
                            5, 5);
}

void framelessDialog::showMaximized()
{
    if(m_isMax)
    {
        qDebug() << normalRect;
        setGeometry(normalRect);
        maxAct->setIcon(QtMaterialTheme::icon("navigation","fullscreen"));
        m_isMax = false;
    }
    else
    {
        maxAct->setIcon(QtMaterialTheme::icon("navigation","fullscreen_exit"));
        setGeometry(fullScreenRect);
        m_isMax = true;
    }
}

bool framelessDialog::eventFilter(QObject *watched, QEvent *event)
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
        if (m_bMoveEnable && pressed && m_pQtMaterialAppBar == qobject_cast<QtMaterialAppBar *>(childAt(mouseEvent->pos()))) {
            if(m_isMax)
            {
                setGeometry(point.x()-normalRect.width()/2,point.y(),normalRect.width(),normalRect.height());
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
        if (m_pQtMaterialAppBar == qobject_cast<QtMaterialAppBar *>(childAt(mouseEvent->pos())))
        {
            showMaximized();
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
