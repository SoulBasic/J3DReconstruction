#ifndef DIALOG_DENSIFYPOINTCLOUD_H
#define DIALOG_DENSIFYPOINTCLOUD_H

#include <QDialog>
#include <qfiledialog.h>
#include "Global.h"
#include <qmessagebox.h>
#include <direct.h>

namespace Ui {
class Dialog_DensifyPointCloud;
}

class Dialog_DensifyPointCloud : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_DensifyPointCloud(QWidget *parent = nullptr);
    ~Dialog_DensifyPointCloud();

private slots:
    void on_pushButton_browseInputDir_clicked();

    void on_pushButton_browseOutputDir_clicked();

    void on_btn_OK_clicked();

    void on_btn_CANCEL_clicked();

private:
    Ui::Dialog_DensifyPointCloud *ui;
};

#endif // DIALOG_DENSIFYPOINTCLOUD_H
