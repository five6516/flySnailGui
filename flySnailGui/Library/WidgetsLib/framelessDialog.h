#ifndef FRAMELESSDIALOG_H
#define FRAMELESSDIALOG_H

#if defined(Lib_PLUGIN_LIBRARY)
#  define Lib_PLUGIN_EXPORT Q_DECL_EXPORT
#else
#  define Lib_PLUGIN_EXPORT Q_DECL_IMPORT
#endif

#include <QDialog>

#include "qtmaterialappbar.h"
#include "qtmaterialiconbutton.h"
#include "lib/qtmaterialtheme.h"

class QLabel;
class QPushButton;
class QHBoxLayout;
class QLayout;
class Lib_PLUGIN_EXPORT framelessDialog : public QDialog
{
    Q_OBJECT
public:
    framelessDialog(QWidget *parent = nullptr);

    QHBoxLayout* getLayout();
    QHBoxLayout* getAppBarLayout();

public Q_SLOTS:
    void showMaximized();
protected:
    /**
     * 1.绘图事件
     * 2.绘制圆角矩形对话框(背景)
     */
    void paintEvent(QPaintEvent *);

    bool eventFilter(QObject *watched, QEvent *event);
    int padding = 8;
    QRect fullScreenRect;
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
    bool m_isMax = false;

private:
    QtMaterialAppBar* m_pQtMaterialAppBar;
    QHBoxLayout *m_pQHBoxLayoutAppMenu;
    QHBoxLayout *m_widgetLayout;
    QtMaterialIconButton* closeAct;
    QtMaterialIconButton* maxAct;
    QtMaterialIconButton* minAct;
};
#endif // FRAMELESSDIALOG_H
