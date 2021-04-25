#ifndef QTMATERIALPROGRESS_H
#define QTMATERIALPROGRESS_H

#if defined(Lib_PLUGIN_LIBRARY)
#  define Lib_PLUGIN_EXPORT Q_DECL_EXPORT
#else
#  define Lib_PLUGIN_EXPORT Q_DECL_IMPORT
#endif

#include <QtWidgets/QProgressBar>
#include "lib/qtmaterialtheme.h"

class QtMaterialProgressPrivate;

class Lib_PLUGIN_EXPORT QtMaterialProgress : public QProgressBar
{
    Q_OBJECT

    Q_PROPERTY(QColor progressColor WRITE setProgressColor READ progressColor)
    Q_PROPERTY(QColor backgroundColor WRITE setProgressColor READ backgroundColor)

public:
    explicit QtMaterialProgress(QWidget *parent = 0);
    ~QtMaterialProgress();

    void setProgressType(Material::ProgressType type);
    Material::ProgressType progressType() const;

    void setUseThemeColors(bool state);
    bool useThemeColors() const;

    void setProgressColor(const QColor &color);
    QColor progressColor() const;

    void setBackgroundColor(const QColor &color);
    QColor backgroundColor() const;

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

    const QScopedPointer<QtMaterialProgressPrivate> d_ptr;

private:
    Q_DISABLE_COPY(QtMaterialProgress)
    Q_DECLARE_PRIVATE(QtMaterialProgress)
};

#endif // QTMATERIALPROGRESS_H
