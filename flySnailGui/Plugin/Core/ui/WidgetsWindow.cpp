#include "WidgetsWindow.h"
#include <QListWidget>
#include <QFileDialog>
#include <QApplication>
#include <QDockWidget>
#include <QStackedWidget>
#include <QGridLayout>
#include "Global.h"
#include "Settings.h"

WidgetsWindow::WidgetsWindow(CPluginManage* pCPluginManage,QWidget *parent) :
    QMainWindow(parent),
    m_pCPluginManage(pCPluginManage)
{
    addLogUI();
    addMenuUI();
    addTreeListUI();
    addWidgetsUI();

    QGridLayout* pQGridLayout = new QGridLayout();
    pQGridLayout->setMargin(0);
    pQGridLayout->setSpacing(0);
    pQGridLayout->addWidget(m_pQStackedWidget);

    QWidget* center = new QWidget();
    setCentralWidget(center);
    center->setLayout(pQGridLayout);
}

WidgetsWindow::~WidgetsWindow()
{

}

void WidgetsWindow::addWidgetsUI()
{
    m_pQStackedWidget = new QStackedWidget();
}

void WidgetsWindow::addMenuUI()
{
    m_pQtMaterialTabs = new QtMaterialTabs();
    m_pQtMaterialTabs->setMaximumHeight(100);
    connect(m_pQtMaterialTabs,&QtMaterialTabs::currentChanged,this,[&](int value){
        m_pQStackedWidget->setCurrentIndex(value);
        for(auto i : m_callBackMenuWidget)
        {
            i(m_pQtMaterialTabs->currentText());
        }
    });

    m_pDockTab = new QDockWidget(this);
    if(!g_ShowTab)
        m_pDockTab->setVisible(false);
    m_pDockTab->setAllowedAreas(Qt::AllDockWidgetAreas);
    m_pDockTab->setWidget(m_pQtMaterialTabs);
    m_pDockTab->setFeatures(QDockWidget::NoDockWidgetFeatures);
    addDockWidget(Qt::TopDockWidgetArea,m_pDockTab);

    QWidget* lTitleBar = m_pDockTab->titleBarWidget();
    QWidget* lEmptyWidget = new QWidget();
    m_pDockTab->setTitleBarWidget(lEmptyWidget);
    delete lTitleBar;
}

void WidgetsWindow::addLogUI()
{
    QtMaterialScrollBar* pQtMaterialScrollBar = new QtMaterialScrollBar();
    m_pQListWidget = new QListWidget();
    m_pQListWidget->setVerticalScrollBar(pQtMaterialScrollBar);
    m_pQListWidget->setAttribute(Qt::WA_TranslucentBackground);
    m_pQListWidget->setForegroundRole(QPalette::Foreground);
    m_pQListWidget->setStyleSheet("QListWidget{margin-left:9px;margin-right:9px;}");
    connect(m_pQListWidget,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(ItemDoubleClick(QListWidgetItem*)));

    m_pDockLog = new QDockWidget(this);
    if(!g_ShowLogWindow)
    {
        m_pDockLog->setVisible(false);
    }
    m_pDockLog->setAllowedAreas(Qt::AllDockWidgetAreas);
    m_pDockLog->setWidget(m_pQListWidget);
    m_pDockLog->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
    addDockWidget(Qt::BottomDockWidgetArea,m_pDockLog);

    QWidget* lTitleBar = m_pDockLog->titleBarWidget();
    QWidget* lEmptyWidget = new QWidget();
    m_pDockLog->setTitleBarWidget(lEmptyWidget);
    delete lTitleBar;
}

void WidgetsWindow::addTreeListUI()
{
    m_pQTreeWidget = new QTreeWidget();
    QFont font = m_pQTreeWidget->font();
    font.setPointSize(13);
    m_pQTreeWidget->setFont(font);
    m_pQTreeWidget->clear();
    m_pQTreeWidget->setHeaderLabel("配置");
    m_pQTreeWidget->expandAll();//展开
    connect(m_pQTreeWidget,SIGNAL(itemPressed(QTreeWidgetItem *,int)),this,SLOT(treeItemPressed(QTreeWidgetItem*,int)));

    m_pDockTreeList = new QDockWidget(this);
    if(!g_ShowTreeList)
        m_pDockTreeList->setVisible(false);
    m_pDockTreeList->setAllowedAreas(Qt::AllDockWidgetAreas);
    m_pDockTreeList->setWidget(m_pQTreeWidget);
    m_pDockTreeList->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
    addDockWidget(Qt::LeftDockWidgetArea,m_pDockTreeList);

    QWidget* lTitleBar = m_pDockTreeList->titleBarWidget();
    QWidget* lEmptyWidget = new QWidget();
    m_pDockTreeList->setTitleBarWidget(lEmptyWidget);
    delete lTitleBar;
}


void WidgetsWindow::loadSMVTPlugin()
{
    //寻找插件
    m_pCPluginManage->findPlugin();

    //加载插件
    m_pCPluginManage->loadPlugin();

    //加入控件界面
    int i=0;
    for(auto& plugin : m_pCPluginManage->m_mapPlugin)
    {
        if(!plugin.second->getEnable())
            continue;

        QWidget* pWidget = plugin.second->pluginWidget();

        if(plugin.second->pluginType() == Plugin_Type::pop)
        {
            pWidget->setAttribute(Qt::WA_DeleteOnClose);
            pWidget->setWindowModality(Qt::ApplicationModal);
            if(QDialog::Accepted == ((QDialog*)pWidget)->exec())
                continue;
            else
                exit(0);
        }

        m_pQStackedWidget->addWidget(pWidget);//加入QStackedWidget
        plugin.second->setIndex(i++);
        m_pQtMaterialTabs->addTab(plugin.second->pluginName());
    }

    m_pQTreeWidget->expandAll();//展开
}

void WidgetsWindow::showTreeWidget()
{
    if(m_pQTreeWidget->isHidden())
    {
        m_pQTreeWidget->show();
    }
    else
    {
        m_pQTreeWidget->hide();
    }
}

QString WidgetsWindow::treeSelectValue()
{
    if(!m_pQTreeWidget->currentItem())
        return "";
    return m_pQTreeWidget->currentItem()->text(0);
}

void WidgetsWindow::addTreeCallBack(std::function<void(const QString& value)> func)
{
    m_callBackTreeWidget.emplace_back(func);
}

void WidgetsWindow::addMenuCallBack(std::function<void(const QString& value)> func)
{
    m_callBackMenuWidget.emplace_back(func);
}

QString WidgetsWindow::currentTree()
{
    return m_pQTreeWidget->currentItem()->text(0);
}

QString WidgetsWindow::currentMenu()
{
    return m_pQtMaterialTabs->currentText();
}

void WidgetsWindow::loadSMVTConfig()
{

    m_pQTreeWidget->expandAll();//展开
}

void WidgetsWindow::ItemDoubleClick(QListWidgetItem* item)
{
    item->setFlags(item->flags() | Qt::ItemIsEditable);
}


bool WidgetsWindow::showLogPlusSlot(QString data, int R,int G,int B,int Size)
{
    (void)Size;
    QListWidgetItem *listItem = new QListWidgetItem(data);
    listItem->setSizeHint(QSize(60, 25));  // use to change the height
    listItem->setForeground(QBrush(QColor(R,G,B)));

    m_pQListWidget->insertItem(m_pQListWidget->count(), listItem);
    m_pQListWidget->scrollToBottom();

    return true;

}

bool WidgetsWindow::showLogSlot(QString data)
{
    QListWidgetItem *listItem = new QListWidgetItem(data);

    m_pQListWidget->insertItem(m_pQListWidget->count(), listItem);
    m_pQListWidget->scrollToBottom();

    return true;
}

void WidgetsWindow::buttonSaveLog()
{
    QString fileName = QFileDialog::getSaveFileName(
        this,
        tr("select a file."),
        QString().asprintf("%s\\log.txt",qApp->applicationDirPath().toStdString().c_str()),
        tr("log(*.txt);All files(*.*)"));

    QFile file(fileName);
    bool bOpen=file.open(QIODevice::ReadWrite|QIODevice::Truncate|QIODevice::Text);
    if (!bOpen)
    {
        showLogSlot("save to file Error,openfile error");
        return;
    }

    for (int j = 0; j < m_pQListWidget->count(); j++)
    {
        QString itemText = m_pQListWidget->item(j)->text() + "\n";
        file.write(itemText.toStdString().c_str());
    }
    file.close();
}

void WidgetsWindow::buttonClearLog()
{
    m_pQListWidget->clear();
}

void WidgetsWindow::treeItemPressed(QTreeWidgetItem *item, int column)
{
    for(auto i : m_callBackTreeWidget)
    {
        i(item->text(column));
    }
}
