#include "dialog_fullauto.h"
#include "ui_dialog_fullauto.h"

Dialog_FullAuto::Dialog_FullAuto(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::Dialog_FullAuto)
{
	ui->setupUi(this);
}

Dialog_FullAuto::~Dialog_FullAuto()
{
	delete ui;
}

void Dialog_FullAuto::on_pushButton_browseInputDir_clicked()
{
	Global::fullauto_InPutDir = QFileDialog::getExistingDirectory(this, "浏览源图片路径 ", "", NULL);
	ui->lineEdit_inputDir->setText(Global::fullauto_InPutDir);
}

void Dialog_FullAuto::on_pushButton_browseOutputDir_clicked()
{
	Global::fullauto_OutputDir = QFileDialog::getExistingDirectory(this, "浏览工程输出路径 ", "", NULL);
	ui->lineEdit_OutputDir->setText(Global::fullauto_OutputDir);
}

void Dialog_FullAuto::on_btn_OK_clicked()
{
	QString kMatrix, describerMethod, quality, upright, forceCompute, geometricModel
		, distanceRatio, forceMatch, maxCacheSizematchesDir, sfmOutputDir, triangulationMethod, resectionMethod
		, densifyInputDir, densifyOutputDir, densifyWorkingDir, reconstructMeshInputDir, reconstructMeshOutputDir
		, reconstructMeshWorkingDir, textureMeshInputDir, textureMeshOutputDir, textureMeshWorkingDir;
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

	Global::fullauto_InPutDir = ui->lineEdit_inputDir->text();
	Global::fullauto_OutputDir = ui->lineEdit_OutputDir->text();

	switch (ui->comboBox_describer->currentIndex())
	{
	case 0: {
		describerMethod = "SIFT";
		break;
	}
	case 1: {
		describerMethod = "SIFT_ANATOMY";
		break;
	}
	case 2: {
		describerMethod = "AKAZE_FLOAT";
		break;
	}
	case 3: {
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

	triangulationMethod = triangulationMethod.number(ui->comboBox_triangulationMethod->currentIndex());

	resectionMethod = resectionMethod.number(ui->comboBox_resectionMethod->currentIndex());

	densifyInputDir = ui->lineEdit_OutputDir->text() + "/SparseCloud.j3d";
	densifyOutputDir = ui->lineEdit_OutputDir->text() + "/DenseCloud.j3d";
	densifyWorkingDir = ui->lineEdit_OutputDir->text();

	reconstructMeshInputDir = ui->lineEdit_OutputDir->text() + "/DenseCloud.j3d";
	reconstructMeshOutputDir = ui->lineEdit_OutputDir->text() + "/TIN_Mesh.j3d";
	reconstructMeshWorkingDir = ui->lineEdit_OutputDir->text();

	textureMeshInputDir = ui->lineEdit_OutputDir->text() + "/TIN_Mesh.j3d";
	textureMeshOutputDir = ui->lineEdit_OutputDir->text() + "/TEXTURE_Mesh.j3d";
	textureMeshWorkingDir = ui->lineEdit_OutputDir->text();

	_mkdir("C:\\ProgramData\\J3DEngine");

	QFile cmdcache("C:\\ProgramData\\J3DEngine\\fullautoCmdCache.tmp");

	if (cmdcache.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
	{
		QString head = "matchfeature\n";
		cmdcache.write(head.toUtf8());
		cmdcache.write(Global::fullauto_InPutDir.toUtf8());
		cmdcache.write("\n");
		cmdcache.write(Global::fullauto_OutputDir.toUtf8());
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
		head = "sfmandsfp\n";
		cmdcache.write(head.toUtf8());
		cmdcache.write(Global::fullauto_OutputDir.toUtf8());
		cmdcache.write("\n");
		cmdcache.write(Global::fullauto_OutputDir.toUtf8());
		cmdcache.write("\n");
		cmdcache.write(triangulationMethod.toUtf8());
		cmdcache.write("\n");
		cmdcache.write(resectionMethod.toUtf8());
		cmdcache.write("\n");
		head = "densifypointcloud\n";
		cmdcache.write(head.toUtf8());
		cmdcache.write(densifyInputDir.toUtf8());
		cmdcache.write("\n");
		cmdcache.write(densifyWorkingDir.toUtf8());
		cmdcache.write("\n");
		cmdcache.write(densifyOutputDir.toUtf8());
		cmdcache.write("\n");
		head = "reconstructmesh\n";
		cmdcache.write(head.toUtf8());
		cmdcache.write(reconstructMeshInputDir.toUtf8());
		cmdcache.write("\n");
		cmdcache.write(reconstructMeshWorkingDir.toUtf8());
		cmdcache.write("\n");
		cmdcache.write(reconstructMeshOutputDir.toUtf8());
		cmdcache.write("\n");
		head = "texturemesh\n";
		cmdcache.write(head.toUtf8());
		cmdcache.write(textureMeshInputDir.toUtf8());
		cmdcache.write("\n");
		cmdcache.write(textureMeshWorkingDir.toUtf8());
		cmdcache.write("\n");
		cmdcache.write(textureMeshOutputDir.toUtf8());
		cmdcache.write("\n");
		cmdcache.write(ui->comboBox->currentText().toUtf8());
		cmdcache.write("\n");
		cmdcache.close();

		QMessageBox::information(NULL, "完成", "配置完成", QMessageBox::Yes, NULL);
		PostThreadMessageA(Global::engineTid, CMD_FULLAUTO, 0, 0);
		Global::autoTasking = true;
		this->close();
	}
	else
	{
		QMessageBox::information(NULL, "错误", "无法访问缓存文件，请检查权限 ", QMessageBox::Yes, NULL);
	}
}

void Dialog_FullAuto::on_btn_CANCEL_clicked()
{
	this->close();
}
