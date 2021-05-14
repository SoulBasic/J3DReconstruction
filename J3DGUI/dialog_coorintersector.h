#ifndef DIALOG_COORINTERSECTOR_H
#define DIALOG_COORINTERSECTOR_H

#include <QDialog>

namespace Ui {
class Dialog_CoorIntersector;
}

class Dialog_CoorIntersector : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_CoorIntersector(QWidget *parent = nullptr);
    ~Dialog_CoorIntersector();

private:
    Ui::Dialog_CoorIntersector *ui;
};

#endif // DIALOG_COORINTERSECTOR_H
