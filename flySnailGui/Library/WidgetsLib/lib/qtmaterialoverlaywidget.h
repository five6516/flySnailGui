#ifndef QTMATERIALOVERLAYWIDGET_H
#define QTMATERIALOVERLAYWIDGET_H

#if defined(Lib_PLUGIN_LIBRARY)
#  define Lib_PLUGIN_EXPORT Q_DECL_EXPORT
#else
#  define Lib_PLUGIN_EXPORT Q_DECL_IMPORT
#endif

#include <QtWidgets/QWidget>

class Lib_PLUGIN_EXPORT QtMaterialOverlayWidget : public QWidget
{
    Q_OBJECT

public:
    explicit QtMaterialOverlayWidget(QWidget *parent = 0);
    ~QtMaterialOverlayWidget();

protected:
    bool event(QEvent *event) Q_DECL_OVERRIDE;
    bool eventFilter(QObject *obj, QEvent *event) Q_DECL_OVERRIDE;

    virtual QRect overlayGeometry() const;

private:
    Q_DISABLE_COPY(QtMaterialOverlayWidget)
};

#endif // QTMATERIALOVERLAYWIDGET_H
