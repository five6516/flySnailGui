#ifndef QTMATERIALAPPBAR_H
#define QTMATERIALAPPBAR_H

#if defined(Lib_PLUGIN_LIBRARY)
#  define Lib_PLUGIN_EXPORT Q_DECL_EXPORT
#else
#  define Lib_PLUGIN_EXPORT Q_DECL_IMPORT
#endif

#include <QtWidgets/QWidget>
#include <QtWidgets/QHBoxLayout>

class QtMaterialAppBarPrivate;

class Lib_PLUGIN_EXPORT QtMaterialAppBar : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(QColor foregroundColor WRITE setForegroundColor READ foregroundColor)
    Q_PROPERTY(QColor backgroundColor WRITE setBackgroundColor READ backgroundColor)

public:
    explicit QtMaterialAppBar(QWidget *parent = 0);
    ~QtMaterialAppBar();

    QSize sizeHint() const Q_DECL_OVERRIDE;

    void setUseThemeColors(bool value);
    bool useThemeColors() const;

    void setForegroundColor(const QColor &color);
    QColor foregroundColor() const;

    void setBackgroundColor(const QColor &color);
    QColor backgroundColor() const;

    inline QHBoxLayout *appBarLayout() const;

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

    const QScopedPointer<QtMaterialAppBarPrivate> d_ptr;

private:
    Q_DISABLE_COPY(QtMaterialAppBar)
    Q_DECLARE_PRIVATE(QtMaterialAppBar)
};

inline QHBoxLayout *QtMaterialAppBar::appBarLayout() const
{
    return static_cast<QHBoxLayout *>(layout());
}

#endif // QTMATERIALAPPBAR_H
