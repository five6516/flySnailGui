#ifndef WIDGETSWINDOW_H
#define WIDGETSWINDOW_H

#include <QMainWindow>
#include <QApplication>
#include "PluginManage/PluginManage.h"
#include "qtmaterialscrollbar.h"
#include "qtmaterialtabs.h"
#include "../CorePluginExport.h"

class QListWidgetItem;
class QListWidget;
class QStackedWidget;
class QTreeWidgetItem;
class QTreeWidget;
class QDockWidget;
class WidgetsWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit WidgetsWindow(CPluginManage* pCPluginManage,QWidget *parent = nullptr);
    ~WidgetsWindow();

    void loadSMVTConfig();
    void loadSMVTPlugin();
    void showTreeWidget();
    QString treeSelectValue();
    void addTreeCallBack(std::function<void(const QString& value)> func);
    void addMenuCallBack(std::function<void(const QString& value)> func);
    QString currentTree();
    QString currentMenu();
private:

    void addLogUI();
    void addMenuUI();
    void addWidgetsUI();
    void addTreeListUI();

    QDockWidget * m_pDockLog;
    QDockWidget * m_pDockTab;
    QDockWidget * m_pDockTreeList;

    QListWidget* m_pQListWidget;//Log窗口
    QtMaterialTabs* m_pQtMaterialTabs;
    QStackedWidget* m_pQStackedWidget;//放置插件窗口
    QTreeWidget* m_pQTreeWidget;//插件列表

    CPluginManage* m_pCPluginManage = nullptr;
    std::vector<std::function<void(const QString& value)>> m_callBackTreeWidget;
    std::vector<std::function<void(const QString& value)>> m_callBackMenuWidget;
public slots:
    void treeItemPressed(QTreeWidgetItem *item,int column);
    void buttonSaveLog();
    void buttonClearLog();
    bool showLogPlusSlot(QString data, int R,int G,int B,int Size);
    bool showLogSlot(QString data);
    void ItemDoubleClick(QListWidgetItem* item);
};

#endif // WIDGETSWINDOW_H
