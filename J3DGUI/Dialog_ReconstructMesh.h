#ifndef DIALOG_RECONSTRUCTMESH_H
#define DIALOG_RECONSTRUCTMESH_H

#include <QDialog>
#include "Global.h"
#include <qfiledialog.h>
#include <qmessagebox.h>
#include <direct.h>

namespace Ui {
class Dialog_ReconstructMesh;
}

class Dialog_ReconstructMesh : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_ReconstructMesh(QWidget *parent = nullptr);
    ~Dialog_ReconstructMesh();

private slots:
    void on_btn_OK_clicked();

    void on_btn_CANCEL_clicked();

    void on_pushButton_browseInputDir_clicked();

    void on_pushButton_browseOutputDir_clicked();

private:
    Ui::Dialog_ReconstructMesh *ui;
};

#endif // DIALOG_RECONSTRUCTMESH_H
