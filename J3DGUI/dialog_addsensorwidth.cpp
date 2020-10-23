#include "dialog_addsensorwidth.h"
#include "ui_dialog_addsensorwidth.h"

Dialog_addsensorwidth::Dialog_addsensorwidth(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::Dialog_addsensorwidth)
{
	ui->setupUi(this);
}

Dialog_addsensorwidth::~Dialog_addsensorwidth()
{
	delete ui;
}

void Dialog_addsensorwidth::on_btn_OK_clicked()
{
	if (ui->lineEdit_model->text() == "")
	{
		QMessageBox::critical(this, u8"错误 ", u8"未输入相机型号 ", QMessageBox::Ok, QMessageBox::Ok);
		return;
	}
	if (ui->lineEdit_focus->text() == "")
	{
		QMessageBox::critical(this, u8"错误 ", u8"未输入相机焦距 ", QMessageBox::Ok, QMessageBox::Ok);
		return;
	}
	if (ui->lineEdit_fov->text() == "")
	{
		QMessageBox::critical(this, u8"错误 ", u8"未输入相机视场角 ", QMessageBox::Ok, QMessageBox::Ok);
		return;
	}
	double sensorWidth = 2 * (ui->lineEdit_focus->text().toDouble() * tan((0.5 * ui->lineEdit_fov->text().toDouble()) / 57.296));
	QString txt = ui->lineEdit_model->text() + ";" + QString::number(sensorWidth);
	QFile swd("SenWidDB.txt");
	if (swd.open(QIODevice::ReadWrite | QIODevice::Append | QIODevice::Text))
	{
		swd.write("\n");
		swd.write(txt.toUtf8());
		swd.close();
		QMessageBox::information(this, u8"成功", u8"成功输入相机参数 ", QMessageBox::Ok, QMessageBox::Ok);
		this->close();
	}
	else
	{
		QMessageBox::information(this, u8"失败", u8"无法打开SensorWidthDB文件 ", QMessageBox::Ok, QMessageBox::Ok);
	}


}

void Dialog_addsensorwidth::on_btn_CANCEL_clicked()
{
	this->close();
}
