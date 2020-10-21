#ifndef DIALOG_TEXTUREMESH_H
#define DIALOG_TEXTUREMESH_H

#include <QDialog>
#include "Global.h"
#include <qfiledialog.h>
#include <qmessagebox.h>
#include <direct.h>
namespace Ui {
	class Dialog_TextureMesh;
}

class Dialog_TextureMesh : public QDialog
{
	Q_OBJECT

public:
	explicit Dialog_TextureMesh(QWidget *parent = nullptr);
	~Dialog_TextureMesh();

private slots:
	void on_btn_OK_clicked();

	void on_btn_CANCEL_clicked();

	void on_pushButton_browseInputDir_clicked();

	void on_pushButton_browseOutputDir_clicked();

private:
	Ui::Dialog_TextureMesh *ui;
};

#endif // DIALOG_TEXTUREMESH_H
