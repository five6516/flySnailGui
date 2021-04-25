#ifndef CHECKDELEGATE_H
#define CHECKDELEGATE_H

#include <QObject>
#include <QStyledItemDelegate>

//数据项委托
class CheckDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    CheckDelegate(QObject *parent = 0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const override;
    void updateEditorGeometry(QWidget *editor,
        const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};


#endif // CHECKDELEGATE_H
