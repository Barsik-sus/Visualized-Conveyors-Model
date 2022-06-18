#include "delegate.h"
#include <QSpinBox>

QWidget* Delegate::createEditor(QWidget *parent, const QStyleOptionViewItem & option,
                      const QModelIndex & index) const
{
//    QLineEdit *lineEdit = new QLineEdit(parent);
//    // Set validator
//    QIntValidator *validator = new QIntValidator(0, 999, lineEdit);
//    lineEdit->setValidator(validator);
//    return lineEdit;
    QSpinBox *editor = new QSpinBox(parent);
    editor->setFrame(false);
    const auto& column = index.column();
    if (column < static_cast<int>(columnMinAndMax.size()))
    {
        editor->setMinimum(columnMinAndMax[column].first);
        editor->setMaximum(columnMinAndMax[column].second);
    }

    return editor;
}
