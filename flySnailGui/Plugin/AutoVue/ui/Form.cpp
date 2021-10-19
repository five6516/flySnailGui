#include "Form.h"
#include <QString>
#include <QImage>
#include <QPixmap>
#include <QFile>
#include <QCloseEvent>
#include "CorePluginExport.h"
#include "Global.h"

#include <QStandardPaths>
#include <QDir>
#include <QGridLayout>
#include <QMdiArea>
#include "mdichild.h"

Form::Form(QWidget *parent) :
    QWidget(parent)
{
    QGridLayout* pMainQGridLayout = new QGridLayout();
    pMainQGridLayout->setSpacing(0);
    pMainQGridLayout->setMargin(0);

    m_pQtMaterialAppBar = new QtMaterialAppBar();
    m_pQMdiArea = new QMdiArea;
    m_pQMdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_pQMdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    pMainQGridLayout->addWidget(m_pQtMaterialAppBar);
    pMainQGridLayout->addWidget(m_pQMdiArea);



    setLayout(pMainQGridLayout);

    MdiChild* pMdiChild = new MdiChild();
    m_pQMdiArea->addSubWindow(pMdiChild);
    pMdiChild->newFile();
    pMdiChild->showMaximized();
    pMdiChild->show();


}

Form::~Form()
{
}

void Form::closeEvent(QCloseEvent *event)
{
    m_pQMdiArea->closeAllSubWindows();
    if (m_pQMdiArea->currentSubWindow()) {
        event->ignore();
    } else {
       // writeSettings();
        event->accept();
    }
    m_pQMdiArea->deleteLater();

}
