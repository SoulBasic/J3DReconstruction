#include "dialog_pointinfo.h"
#include "ui_dialog_pointinfo.h"
#include "dialog_coorintersector.h"
Dialog_PointInfo::Dialog_PointInfo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_PointInfo)
{
	setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    ui->setupUi(this);
}

Dialog_PointInfo::~Dialog_PointInfo()
{
    delete ui;
}

void Dialog_PointInfo::on_pushButton_cancel_clicked()
{
	this->close();
}

void Dialog_PointInfo::on_pushButton_OK_clicked()
{
	Dialog_CoorIntersector* _parent = (Dialog_CoorIntersector*)_p;
	_parent->addPoint(ui->lineEdit_xi->text().toDouble(), ui->lineEdit_yi->text().toDouble(), ui->lineEdit_zi->text().toDouble());
	this->close();
}

void Dialog_PointInfo::setX(int x) { ui->lineEdit_x->setText(QString::number(x)); }
void Dialog_PointInfo::setY(int y) { ui->lineEdit_y->setText(QString::number(y)); }
void Dialog_PointInfo::setXi(double x) 
{
	ui->lineEdit_xi->setText(QString::number(x,'g',10)); 
}
void Dialog_PointInfo::setYi(double y) 
{ 
	ui->lineEdit_yi->setText(QString::number(y, 'g', 10));
}
void Dialog_PointInfo::setZi(double z) 
{ 
	ui->lineEdit_zi->setText(QString::number(z, 'g', 10));
}

void Dialog_PointInfo::clearXYZi()
{
	ui->lineEdit_xi->setText("");
	ui->lineEdit_yi->setText("");
	ui->lineEdit_zi->setText("");
}

