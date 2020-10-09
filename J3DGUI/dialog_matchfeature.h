
#ifndef DIALOG_MATCHFEATURE_H
#define DIALOG_MATCHFEATURE_H

#include <QDialog>
#include <iostream>
#include <qfile.h>
#include <qmessagebox.h>
#include "Global.h"
#include <qfiledialog.h>
#include <direct.h>
#include <math.h>

namespace Ui {
class Dialog_MatchFeature;
}

class Dialog_MatchFeature : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_MatchFeature(QWidget *parent = nullptr);
    ~Dialog_MatchFeature();

private slots:


    void on_btn_CANCEL_clicked();

    void on_btn_OK_clicked();

	void pushButton_browseInputDir_clicked();

	void pushButton_browseOutputDir_clicked();

private:
    Ui::Dialog_MatchFeature *ui;

};

#endif // DIALOG_MATCHFEATURE_H
