#ifndef DELEGATE_H
#define DELEGATE_H

#include <QLineEdit>
#include <QItemDelegate>

class Delegate : public QItemDelegate
{
public:
    std::vector<std::pair<int, int>> columnMinAndMax;
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem & option,
                      const QModelIndex & index) const;
};

#endif // DELEGATE_H
