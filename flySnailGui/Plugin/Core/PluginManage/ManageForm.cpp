#include "ManageForm.h"
#include <QDebug>
#include "Global.h"
#include <QCheckBox>
#include "CheckDelegate.h"
#include "PluginManage.h"


static const int SortRole = Qt::UserRole + 100;

class SortFilterProxyModel
        :public QSortFilterProxyModel
{
    using QSortFilterProxyModel::QSortFilterProxyModel;
    // QSortFilterProxyModel interface
protected:
    virtual bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override{
        if (!source_parent.isValid()) {
            // category items should be visible if any of its children match
            const QRegExp &regexp = filterRegExp();
            const QModelIndex &categoryIndex = sourceModel()->index(source_row, 0, source_parent);
            if (regexp.indexIn(sourceModel()->data(categoryIndex, filterRole()).toString()) != -1)
                return true;
            const int rowCount = sourceModel()->rowCount(categoryIndex);
            const int columnCount = sourceModel()->columnCount(categoryIndex);
            for (int row = 0; row < rowCount; ++row) {
                for (int column = 0; column < columnCount; ++column) {
                    if (regexp.indexIn(sourceModel()->data(
                                           sourceModel()->index(row, column, categoryIndex),
                                           filterRole()).toString()) != -1)
                        return true;
                }
            }
            return false;
        }
        return QSortFilterProxyModel::filterAcceptsRow(source_row, source_parent);
    }
};


ManageForm::ManageForm(CPluginManage* pCPluginManage,QWidget *parent) :
    framelessDialog(parent),
    m_pCPluginManage(pCPluginManage)
{
    setMinimumSize(800,600);
    normalRect = QRect(parent->x(),parent->y(),parent->width(),parent->height());
    setGeometry(normalRect);

    QLabel* pTitleLabel = new QLabel("插件管理");
    getAppBarLayout()->addWidget(pTitleLabel);

    m_pQTreeView = new QTreeView();
    m_pQTreeView->setAlternatingRowColors(true);
    m_pQTreeView->setIndentation(20);
    m_pQTreeView->setUniformRowHeights(true);
    m_pQTreeView->setSortingEnabled(true);
    m_pQTreeView->setSelectionMode(QAbstractItemView::SingleSelection);
    m_pQTreeView->setSelectionBehavior(QAbstractItemView::SelectRows);

    m_model = new QStandardItemModel(m_pQTreeView);
    m_model->setHorizontalHeaderLabels(QStringList() << tr("Name") << tr("Load") << tr("Version") << tr("Type"));

    m_sortModel = new SortFilterProxyModel(this);
    m_sortModel->setSourceModel(m_model);
    m_sortModel->setSortRole(SortRole);
    m_sortModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    m_sortModel->setFilterKeyColumn(-1/*all*/);

    //CheckDelegate* delegate = new CheckDelegate;
    //m_pQTreeView->setItemDelegate(delegate);

    updatePlugins();

    QHeaderView *header = m_pQTreeView->header();
    header->setDefaultSectionSize(180);
    header->setMinimumSectionSize(35);
    header->setStretchLastSection(true);               //最后一列自适应宽度
    header->setDefaultAlignment(Qt::AlignCenter);      //列头文字默认居中对齐
    header->setSortIndicator(0, Qt::AscendingOrder);

    m_pQTreeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_pQTreeView->setModel(m_sortModel);
    m_pQTreeView->expandAll();

    connect(m_model,&QStandardItemModel::itemChanged,this,&ManageForm::treeitemCheckchildchanged);

    QHBoxLayout* pQHBoxLayout = new QHBoxLayout();
    m_pQLineEdit = new QLineEdit();
    m_pQLineEdit->setPlaceholderText("筛选");
    connect(m_pQLineEdit, &QLineEdit::textChanged, this,[&](const QString &filter){
        m_sortModel->setFilterRegExp(filter);
        m_pQTreeView->expandAll();
    });
    m_pQLabel = new QLabel();
    pQHBoxLayout->addWidget(m_pQLineEdit);
    pQHBoxLayout->addWidget(m_pQLabel);

    QVBoxLayout* pQVBoxLayout = new QVBoxLayout();
    pQVBoxLayout->setSpacing(8);
    pQVBoxLayout->setMargin(8);
    pQVBoxLayout->addLayout(pQHBoxLayout);
    pQVBoxLayout->addWidget(m_pQTreeView);

    getLayout()->addLayout(pQVBoxLayout);
}

ManageForm::~ManageForm()
{

}

void ManageForm::treeItemCheckChildChanged(QStandardItem * item)
{
    if(nullptr == item)
        return;
    Qt::CheckState siblingState = checkSibling(item);
    QStandardItem * parentItem = item->parent();
    if(nullptr == parentItem)
        return;

    parentItem = m_model->item(parentItem->row(),1);
    if(Qt::PartiallyChecked == siblingState)
    {
        if(parentItem->isCheckable() && parentItem->isTristate())
            parentItem->setCheckState(Qt::PartiallyChecked);
    }
    else if(Qt::Checked == siblingState)
    {
        if(parentItem->isCheckable())
            parentItem->setCheckState(Qt::Checked);
    }
    else
    {
        if(parentItem->isCheckable())
            parentItem->setCheckState(Qt::Unchecked);
    }
    treeItemCheckChildChanged(parentItem);
}

Qt::CheckState ManageForm::checkSibling(QStandardItem * item)
{
    //先通过父节点获取兄弟节点
    QStandardItem * parent = item->parent();
    if(nullptr == parent)
        return item->checkState();
    int brotherCount = parent->rowCount();
    int checkedCount(0),unCheckedCount(0);
    Qt::CheckState state;
    for(int i=0;i<brotherCount;++i)
    {
        QStandardItem* siblingItem = parent->child(i,1);
        state = siblingItem->checkState();
        if(Qt::PartiallyChecked == state)
            return Qt::PartiallyChecked;
        else if(Qt::Unchecked == state)
            ++unCheckedCount;
        else
            ++checkedCount;
        if(checkedCount>0 && unCheckedCount>0)
            return Qt::PartiallyChecked;
    }
    if(unCheckedCount>0)
        return Qt::Unchecked;
    return Qt::Checked;
}

void ManageForm::treeItemCheckAllChild(QStandardItem * item, bool check)
{
    if(item == nullptr)
        return;
    int rowCount = item->rowCount();
    for(int i=0;i<rowCount;++i)
    {
        QStandardItem* childItems = item->child(i,1);//第二列
        treeItemCheckAllChildRecursion(childItems,check);
    }
    if(item->isCheckable())
        item->setCheckState(check ? Qt::Checked : Qt::Unchecked);
}

void ManageForm::treeItemCheckAllChildRecursion(QStandardItem * item,bool check)
{
    if(item == nullptr)
        return;
    int rowCount = item->rowCount();
    for(int i=0;i<rowCount;++i)
    {
        QStandardItem* childItems = item->child(i,1);
        treeItemCheckAllChildRecursion(childItems,check);
    }
    if(item->isCheckable())
        item->setCheckState(check ? Qt::Checked : Qt::Unchecked);
}

void ManageForm::treeitemCheckchildchanged(QStandardItem *item)
{
    if (item == nullptr)
        return;

    if (item->isCheckable())
    {
        //如果条目是存在复选框的，那么就进行下面的操作
        Qt::CheckState state = item->checkState(); //获取当前的选择状态
        if (item->isTristate())
        {
             //如果条目是三态的，说明可以对子目录进行全选和全不选的设置
            if (state != Qt::PartiallyChecked)
            {
                //当前是选中状态，需要对其子项目进行全选
                treeItemCheckAllChild(m_model->item(item->row()), state == Qt::Checked ? true : false );
            }
        }
        else
        {
            treeItemCheckChildChanged(item);
        }
    }

    //
    if(m_pQLabel)
        m_pQLabel->setText("需要重启");
}

void ManageForm::closeEvent(QCloseEvent *event)
{
    (void)event;
    for(auto& plugin : m_pCPluginManage->m_mapPlugin)
    {
        for (int i=0;i<m_model->rowCount();i++)//遍历目录
        {
            QStandardItem *Item = m_model->item(i);
            for (int j=0;j<Item->rowCount();j++)//遍历子项
            {
                QStandardItem *pluginItem=Item->child(j);
                if(QString(plugin.first.c_str()) == pluginItem->text())
                {
                    if(Item->child(j,1)->checkState() == Qt::CheckState::Checked)
                        plugin.second->setEnable(true);
                    else
                        plugin.second->setEnable(false);
                }
            }
        }
    }

    m_pCPluginManage->writeSetting();
}

void ManageForm::updatePlugins()
{
    /* 插入 */
    for(auto& plugin : m_pCPluginManage->m_mapPlugin)
    {
        QList<QStandardItem*> items = m_model->findItems(QString(plugin.second->pluginContent()));

        if(items.size() == 0)
        {
            QList<QStandardItem*> contentList;
            QStandardItem* item1 = new QStandardItem(plugin.second->pluginContent());
            QStandardItem* item2 = new QStandardItem("");
            item2->setCheckable(true);
            item2->setTristate(true);//三状态
            QStandardItem* item3 = new QStandardItem("");
            QStandardItem* item4 = new QStandardItem("");
            contentList.append(item1);
            contentList.append(item2);
            contentList.append(item3);
            contentList.append(item4);
            m_model->appendRow(contentList);
            items = m_model->findItems(QString(plugin.second->pluginContent()));
        }
        if(items.size() != 0)
        {
            QList<QStandardItem*> pluginList;
            QStandardItem* pluginitem1 = new QStandardItem(plugin.second->pluginName());
            QStandardItem* pluginitem2 = new QStandardItem("");
            pluginitem2->setCheckable(true);

            if(plugin.second->getEnable())
                pluginitem2->setCheckState(Qt::CheckState::Checked);
            else
                pluginitem2->setCheckState(Qt::CheckState::Unchecked);

            QStandardItem* pluginitem3 = new QStandardItem(plugin.second->pluginVersion());

            auto typeToStr = [](Plugin_Type type)->QString{
                switch (type) {
                    case core:return "core";
                    case normal:return "normal";
                    case pop:return "pop";
                    case server:return "server";
                    default:return "normal";
                }
            };
            QStandardItem* pluginitem4 = new QStandardItem(typeToStr(plugin.second->pluginType()));
            pluginList.append(pluginitem1);
            pluginList.append(pluginitem2);
            pluginList.append(pluginitem3);
            pluginList.append(pluginitem4);
            items.first()->appendRow(pluginList);
            treeitemCheckchildchanged(pluginitem2);
        }
    }
}
