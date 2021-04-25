#ifndef QTMATERIALCHECKBOX_H
#define QTMATERIALCHECKBOX_H

#if defined(Lib_PLUGIN_LIBRARY)
#  define Lib_PLUGIN_EXPORT Q_DECL_EXPORT
#else
#  define Lib_PLUGIN_EXPORT Q_DECL_IMPORT
#endif

#include "lib/qtmaterialcheckable.h"

class QtMaterialCheckBoxPrivate;

class Lib_PLUGIN_EXPORT QtMaterialCheckBox : public QtMaterialCheckable
{
    Q_OBJECT

public:
    explicit QtMaterialCheckBox(QWidget *parent = 0);
    ~QtMaterialCheckBox();

private:
    Q_DISABLE_COPY(QtMaterialCheckBox)
    Q_DECLARE_PRIVATE(QtMaterialCheckBox)
};

#endif // QTMATERIALCHECKBOX_H
