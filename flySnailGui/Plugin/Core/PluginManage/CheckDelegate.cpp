#include "CheckDelegate.h"
#include <QCheckBox>
#include <QComboBox>
#include "Global.h"
CheckDelegate::CheckDelegate(QObject *parent): QStyledItemDelegate(parent)
{

}

QWidget *CheckDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                      const QModelIndex &index) const
{
    (void)option;
    if(index.column() == 1) //只对第1列采用此方法编辑
    {
        QCheckBox* box = new QCheckBox(parent);
        box->setChecked(true);
        //box->addItems(QStringList()<<"优"<<"良"<<"差");
        return box;
    }

    return NULL;
}

void CheckDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    bool value = index.model()->data(index, Qt::EditRole).toBool();
    QCheckBox* box = static_cast<QCheckBox*>(editor);
    box->setChecked(value);
}

void CheckDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                  const QModelIndex &index) const
{
    QCheckBox* box = static_cast<QCheckBox*>(editor);
    model->setData(index, box->isChecked(), Qt::EditRole);
}

void CheckDelegate::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    (void)index;
    editor->setGeometry(option.rect);
}
