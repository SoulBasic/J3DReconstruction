#include "dialog_coor.h"
#include "ui_dialog_coor.h"
// WGS84 Ellipsoid
#define WGS84_A 6378137.0      // major axis
#define WGS84_B 6356752.314245 // minor axis
#define WGS84_E 0.0818191908   // first eccentricity
dialog_coor::dialog_coor(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::dialog_coor)
{
	ui->setupUi(this);
}

dialog_coor::~dialog_coor()
{
	WinExec("taskkill /f /im CoorIntersector.exe", SW_HIDE);
	delete ui;
}
void dialog_coor::on_pushButton_browseOutputDir_clicked()
{
	QString dir = QFileDialog::getExistingDirectory(this, u8"浏览重建数据文件夹 ", "", NULL);
	if (dir == "")
	{
		return;
	}
	ui->lineEdit_filename->setText(dir);
}
void dialog_coor::on_pushButton_browseTiffDir_clicked()
{
	QString fileName = QFileDialog::getOpenFileName(NULL,
		u8"选择DSM文件", ".",
		"Tag Image File Format(*.tif);;All Files(*.*)");
	if (fileName == "")
	{
		return;
	}
	ui->lineEdit_filename_tif->setText(fileName);
}



void dialog_coor::on_button_ok_clicked()
{
	QString workDir = ui->lineEdit_filename->text();
	QString fileName = ui->lineEdit_filename_tif->text();
	QString cmd = "CoorIntersector.exe " + workDir + " " + fileName;
	STARTUPINFO si = { sizeof(si) };
	PROCESS_INFORMATION pi;
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = true;

	if (!CreateProcess(
		NULL,
		(LPSTR)cmd.toStdString().c_str(),
		NULL,
		NULL,
		FALSE,
		CREATE_NEW_CONSOLE,
		NULL,
		NULL, &si, &pi))
	{
		QMessageBox::information(NULL, u8"失败", u8"打开文件失败，CoorIntersector程序文件不存在或不完整 ", QMessageBox::Ok, QMessageBox::Ok);
		return;
	}
}
