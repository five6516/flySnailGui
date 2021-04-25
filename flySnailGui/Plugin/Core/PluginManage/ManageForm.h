#ifndef MANAGEFORM_H
#define MANAGEFORM_H

#include "framelessDialog.h"
#include "PluginSpec.h"
#include <QTreeView>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QLabel>
#include <QHeaderView>
#include <QTreeWidgetItem>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>

class CPluginManage;
class ManageForm : public framelessDialog
{
    Q_OBJECT

public:
    explicit ManageForm(CPluginManage* pCPluginManage,QWidget *parent = nullptr);
    ~ManageForm();

public slots:
    void treeitemCheckchildchanged(QStandardItem * item);
protected:
    void closeEvent(QCloseEvent *event);
private:
    CPluginManage* m_pCPluginManage = nullptr;

    QTreeView* m_pQTreeView = nullptr;
    QLineEdit* m_pQLineEdit = nullptr;
    QLabel* m_pQLabel = nullptr;
    QStandardItemModel* m_model = nullptr;
    QSortFilterProxyModel *m_sortModel = nullptr;
    std::map<std::string,CPluginSpec*> m_mapPlugin;

    void updatePlugins();

    void treeItemCheckAllChild(QStandardItem * item, bool check);
    void treeItemCheckAllChildRecursion(QStandardItem * item,bool check);
    void treeItemCheckChildChanged(QStandardItem * item);
    Qt::CheckState checkSibling(QStandardItem * item);
};

#endif // MANAGEFORM_H
