#ifndef QTMATERIALRADIOBUTTON_H
#define QTMATERIALRADIOBUTTON_H

#if defined(Lib_PLUGIN_LIBRARY)
#  define Lib_PLUGIN_EXPORT Q_DECL_EXPORT
#else
#  define Lib_PLUGIN_EXPORT Q_DECL_IMPORT
#endif

#include "lib/qtmaterialcheckable.h"

class QtMaterialRadioButtonPrivate;

class Lib_PLUGIN_EXPORT QtMaterialRadioButton : public QtMaterialCheckable
{
    Q_OBJECT

public:
    explicit QtMaterialRadioButton(QWidget *parent = 0);
    ~QtMaterialRadioButton();

protected:
    void setupProperties();

private:
    Q_DISABLE_COPY(QtMaterialRadioButton)
    Q_DECLARE_PRIVATE(QtMaterialRadioButton)
};

#endif // QTMATERIALRADIOBUTTON_H
