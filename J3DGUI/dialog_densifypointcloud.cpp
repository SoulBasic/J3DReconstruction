#include "dialog_densifypointcloud.h"
#include "ui_dialog_densifypointcloud.h"

Dialog_DensifyPointCloud::Dialog_DensifyPointCloud(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::Dialog_DensifyPointCloud)
{
	ui->setupUi(this);
}

Dialog_DensifyPointCloud::~Dialog_DensifyPointCloud()
{
	delete ui;
}

void Dialog_DensifyPointCloud::on_pushButton_browseInputDir_clicked()
{
	Global::densifyInputDir = QFileDialog::getExistingDirectory(this, u8"浏览重建结果文件夹 ", "", NULL);
	ui->lineEdit_inputDir->setText(Global::densifyInputDir);
}

void Dialog_DensifyPointCloud::on_pushButton_browseOutputDir_clicked()
{
	Global::densifyOutputDir = QFileDialog::getExistingDirectory(this, u8"浏览输出点云文件目录 ", "", NULL);
	ui->lineEdit_OutputDir->setText(Global::densifyOutputDir);
}

void Dialog_DensifyPointCloud::on_btn_OK_clicked()
{
	if (Global::GetProcessidFromName("J3DEngine.exe") == 0)
	{
		QMessageBox::critical(this, u8"错误 ", u8"未找到J3DEngine进程 ", QMessageBox::Ok, QMessageBox::Ok);
		return;
	}
	else
		Global::connectEngine();


	if (ui->lineEdit_inputDir->text() == "")
	{
		QMessageBox::critical(this, u8"错误 ", u8"未输入重建结果路径 ", QMessageBox::Ok, QMessageBox::Ok);
		return;
	}
	if (ui->lineEdit_OutputDir->text() == "")
	{
		QMessageBox::critical(this, u8"错误 ", u8"未输入输出路径 ", QMessageBox::Ok, QMessageBox::Ok);
		return;
	}

	Global::densifyInputDir = ui->lineEdit_inputDir->text() + "/SparseCloud.J3D";
	Global::densifyOutputDir = ui->lineEdit_OutputDir->text() + "/DenseCloud.J3D";
	Global::densifyWorkingDir = ui->lineEdit_OutputDir->text();

	_mkdir("C:\\ProgramData\\J3DEngine");

	QFile cmdcache("C:\\ProgramData\\J3DEngine\\cmdCache.tmp");

	if (cmdcache.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
	{
		QString head = "densifypointcloud\n";
		cmdcache.write(head.toUtf8());
		cmdcache.write(Global::densifyInputDir.toUtf8());
		cmdcache.write("\n");
		cmdcache.write(Global::densifyWorkingDir.toUtf8());
		cmdcache.write("\n");
		cmdcache.write(Global::densifyOutputDir.toUtf8());
		cmdcache.write("\n");
		cmdcache.close();
		QMessageBox::information(NULL, u8"完成", u8"配置完成 ", QMessageBox::Yes, NULL);
		PostThreadMessageA(Global::engineTid, CMD_EXPORTDENSECLOUD, 0, 0);
		Global::tasking = true;
		this->close();
	}
	else
	{
		QMessageBox::information(NULL, u8"错误", u8"无法访问缓存文件，请检查权限，或使用管理员身份运行 ", QMessageBox::Yes, NULL);
	}

}

void Dialog_DensifyPointCloud::on_btn_CANCEL_clicked()
{
	this->close();
}
