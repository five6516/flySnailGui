#ifndef QTMATERIALAUTOCOMPLETE_H
#define QTMATERIALAUTOCOMPLETE_H

#if defined(Lib_PLUGIN_LIBRARY)
#  define Lib_PLUGIN_EXPORT Q_DECL_EXPORT
#else
#  define Lib_PLUGIN_EXPORT Q_DECL_IMPORT
#endif

#include "qtmaterialtextfield.h"

class QtMaterialAutoCompletePrivate;

class Lib_PLUGIN_EXPORT QtMaterialAutoComplete : public QtMaterialTextField
{
    Q_OBJECT

public:
    explicit QtMaterialAutoComplete(QWidget *parent = 0);
    ~QtMaterialAutoComplete();

    void setDataSource(const QStringList &data);

signals:
    void itemSelected(QString);

protected slots:
    void updateResults(QString text);

protected:
    bool event(QEvent *event) Q_DECL_OVERRIDE;
    bool eventFilter(QObject *watched, QEvent *event) Q_DECL_OVERRIDE;

private:
    Q_DISABLE_COPY(QtMaterialAutoComplete)
    Q_DECLARE_PRIVATE(QtMaterialAutoComplete)
};

#endif // QTMATERIALAUTOCOMPLETE_H
