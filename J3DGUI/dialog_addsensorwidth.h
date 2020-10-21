#ifndef DIALOG_ADDSENSORWIDTH_H
#define DIALOG_ADDSENSORWIDTH_H

#include <QDialog>
#include "Global.h"
#include <qfiledialog.h>
#include <qmessagebox.h>
#include <direct.h>

namespace Ui {
	class Dialog_addsensorwidth;
}

class Dialog_addsensorwidth : public QDialog
{
	Q_OBJECT

public:
	explicit Dialog_addsensorwidth(QWidget *parent = nullptr);
	~Dialog_addsensorwidth();

private slots:
	void on_btn_OK_clicked();

	void on_btn_CANCEL_clicked();

private:
	Ui::Dialog_addsensorwidth *ui;
};

#endif // DIALOG_ADDSENSORWIDTH_H
