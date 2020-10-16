#ifndef DIALOG_FULLAUTO_H
#define DIALOG_FULLAUTO_H

#include <QDialog>
#include "Global.h"
#include <qfiledialog.h>
#include <qmessagebox.h>
#include <direct.h>

namespace Ui {
class Dialog_FullAuto;
}

class Dialog_FullAuto : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_FullAuto(QWidget *parent = nullptr);
    ~Dialog_FullAuto();

private slots:
    void on_pushButton_browseInputDir_clicked();

    void on_pushButton_browseOutputDir_clicked();

    void on_btn_OK_clicked();

    void on_btn_CANCEL_clicked();

private:
    Ui::Dialog_FullAuto *ui;
};

#endif // DIALOG_FULLAUTO_H
