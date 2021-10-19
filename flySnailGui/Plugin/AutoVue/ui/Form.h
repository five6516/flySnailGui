#ifndef FORM_H
#define FORM_H

#include "Plugin.h"

#include "qtmaterialdialog.h"
#include "qtmaterialappbar.h"
#include "qtmaterialiconbutton.h"
#include "qtmaterialflatbutton.h"
#include "lib/qtmaterialtheme.h"

class QWidget;
class QMdiArea;
class QMdiSubWindow;
class QCloseEvent;

class Form : public QWidget
{
    Q_OBJECT

protected:
    void closeEvent(QCloseEvent *event) override;

public:
    explicit Form(QWidget *parent = nullptr);
    ~Form();

private:
    QMdiArea* m_pQMdiArea;
    QtMaterialAppBar* m_pQtMaterialAppBar;
};

#endif // FORM_H
