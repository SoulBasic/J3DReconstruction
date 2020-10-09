#include "dialog_matchfeature.h"
#include "ui_dialog_matchfeature.h"


Dialog_MatchFeature::Dialog_MatchFeature(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_MatchFeature)
{
    ui->setupUi(this);
}
Dialog_MatchFeature::~Dialog_MatchFeature()
{
    delete ui;
}


void Dialog_MatchFeature::on_btn_OK_clicked()
{
	QString imagesDir, outputDir,kMatrix, describerMethod, quality, upright, forceCompute, geometricModel, distanceRatio, forceMatch, maxCacheSize;
	if (Global::GetProcessidFromName("J3DEngine.exe") == 0)
	{
		QMessageBox::critical(this, "错误 ", "未找到J3DEngine进程", QMessageBox::Ok, QMessageBox::Ok);
		return;
	}
	else
		Global::connectEngine();

	


	if (ui->lineEdit_kMatrix->text() == "")
	{
		if (QMessageBox::warning(this, "未输入本征矩阵 ", "忽略本征矩阵参数 ", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes) != QMessageBox::Yes)
		{
			return;
		}
		else
			kMatrix = "NULL";
	}
	else
		kMatrix = ui->lineEdit_kMatrix->text();

	if (ui->lineEdit_inputDir->text() == "")
	{
		QMessageBox::critical(this, "错误 ", "未输入图片路径 ", QMessageBox::Ok, QMessageBox::Ok);
		return;
	}
	if (ui->lineEdit_OutputDir->text() == "")
	{
		QMessageBox::critical(this, "错误 ", "未输入输出路径 ", QMessageBox::Ok, QMessageBox::Ok);
		return;
	}
	
	imagesDir = ui->lineEdit_inputDir->text();
	Global::imagesDir = imagesDir;

	outputDir = ui->lineEdit_OutputDir->text();
	Global::matchesOutputDir = outputDir;

	switch (ui->comboBox_describer->currentIndex())
	{
	case 0:{
		describerMethod = "SIFT";
		break;
	}
	case 1:{
		describerMethod = "SIFT_ANATOMY";
		break;
	}
	case 2:{
		describerMethod = "AKAZE_FLOAT";
		break;
	}
	case 3:{
		describerMethod = "AKAZE_MLDB";
		break;
	}

	}

	switch (ui->comboBox_quality->currentIndex())
	{
	case 0: {
		quality = "NORMAL";
		break;
	}
	case 1: {
		quality = "HIGH";
		break;
	}
	case 2: {
		quality = "ULTRA";
		break;
	}
	}

	upright = QString::number(ui->comboBox_upright->currentIndex());

	forceCompute = QString::number(ui->comboBox_forcecompute->currentIndex());

	switch (ui->comboBox_geometricmodel->currentIndex()) {
	case 0: {
		geometricModel = "f";
		break;
	}
	case 1: {
		geometricModel = "e";
		break;
	}
	case 2: {
		geometricModel = "h";
		break;
	}
	case 3: {
		geometricModel = "a";
		break;
	}
	case 4: {
		geometricModel = "o";
		break;
	}
	case 5: {
		geometricModel = "u";
		break;
	}
	}

	distanceRatio = ui->lineEdit_distanceRatio->text();

	forceMatch = QString::number(ui->comboBox_forcematch->currentIndex());
	
	_mkdir("C:\\ProgramData\\J3DEngine");

	QFile cmdcache("C:\\ProgramData\\J3DEngine\\cmdCache.tmp");

	if (cmdcache.open(QIODevice::WriteOnly|QIODevice::Text|QIODevice::Truncate))
	{
		QString head = "matchfeature\n";
		cmdcache.write(head.toUtf8());
		cmdcache.write(imagesDir.toUtf8());
		cmdcache.write("\n");
		cmdcache.write(outputDir.toUtf8());
		cmdcache.write("\n");
		cmdcache.write(kMatrix.toUtf8());
		cmdcache.write("\n");
		cmdcache.write(describerMethod.toUtf8());
		cmdcache.write("\n");
		cmdcache.write(quality.toUtf8());
		cmdcache.write("\n");
		cmdcache.write(upright.toUtf8());
		cmdcache.write("\n");
		cmdcache.write(forceCompute.toUtf8());
		cmdcache.write("\n");
		cmdcache.write(geometricModel.toUtf8());
		cmdcache.write("\n");
		cmdcache.write(distanceRatio.toUtf8());
		cmdcache.write("\n");
		cmdcache.write(forceMatch.toUtf8());
		cmdcache.write("\n");
		cmdcache.close();
		QMessageBox::information(NULL, "完成", "配置完成", QMessageBox::Yes, NULL);
		PostThreadMessageA(Global::engineTid, CMD_MATCHFEATURES, 0, 0);
		Global::tasking = true;
		this->close();
	}
	else 
	{
        QMessageBox::information(NULL, "错误", "无法访问缓存文件，请检查权限 ", QMessageBox::Yes, NULL);
	}

}
void Dialog_MatchFeature::on_btn_CANCEL_clicked()
{
	this->close();
}

void Dialog_MatchFeature::pushButton_browseInputDir_clicked()
{
    Global::imagesDir = QFileDialog::getExistingDirectory(this, "浏览图片文件夹 ", "", NULL);
	ui->lineEdit_inputDir->setText(Global::imagesDir);
}

void Dialog_MatchFeature::pushButton_browseOutputDir_clicked()
{
    Global::matchesOutputDir = QFileDialog::getExistingDirectory(this, "浏览输出文件夹 ", "", NULL);
	ui->lineEdit_OutputDir->setText(Global::matchesOutputDir);
}

