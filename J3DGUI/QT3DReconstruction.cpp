#include "QT3DReconstruction.h"
using namespace std;
QT3DReconstruction::QT3DReconstruction(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	
	QTimer *timer = new QTimer(this); //this 为parent类, 表示当前窗口

	connect(timer, SIGNAL(timeout()), this, SLOT(timerSlot()));

	timer->start(500); // 1000毫秒, 等于 1 秒
	setWindowFlags(windowFlags()& ~Qt::WindowMaximizeButtonHint);
	setFixedSize(this->width(), this->height());
	PlyIO* ply = new PlyIO("");
	
	
}


void QT3DReconstruction::on_actionMatchFeature_triggered()
{
	dlgmf.exec();
}

void QT3DReconstruction::on_actionSFM_triggered()
{
	dlgsfm.exec();
}


LPCWSTR QT3DReconstruction::LPSTRtoLPCWSTR(char* szStr)
{
	WCHAR wszClassName[256];
	memset(wszClassName, 0, sizeof(wszClassName));
	MultiByteToWideChar(CP_ACP, 0, szStr, strlen(szStr) + 1, wszClassName,
		sizeof(wszClassName) / sizeof(wszClassName[0]));
	return wszClassName;
}


void QT3DReconstruction::on_action_viewMVS_triggered()
{

}

void QT3DReconstruction::on_action_addSensorWidth_triggered()
{
	dlgasw.exec();
}

void QT3DReconstruction::on_action_viewPLY_triggered()
{
	QMessageBox::information(NULL, "暂不支持ply格式，请加载j3d模型","错误", QMessageBox::Yes, NULL);
	return;
	QString fileName = QFileDialog::getOpenFileName(NULL, "ViewPLY", ".", "*.ply");
	if (fileName == "")
		return;
	//QMessageBox::information(NULL, "", filename, QMessageBox::Yes, NULL);

	QByteArray buf = fileName.toLatin1(); // must

	PlyIO* ply = new PlyIO(buf.data());
	if (false == ply->open()) {
		ui.textBrowser->insertPlainText("\n模型加载失败");
		ui.textBrowser->update();
		return;
	}
	delete Global::ply;
	Global::ply = ply;
	//ui.J3DViewerWidget->setPly();

	//QProcess myProcess(this);
	//QString program = "J3DViewer.exe";
	//QStringList arguments;
	//arguments << fileName;
	//myProcess.startDetached(program, arguments, NULL, NULL);

}

void QT3DReconstruction::on_actionDenseCloud_triggered()
{
	dlgdense.exec();
}

void QT3DReconstruction::on_action_reconstrctMesh_triggered()
{
	dlgrm.exec();
}

void QT3DReconstruction::timerSlot()
{
	
    if (Global::GetProcessidFromName("J3DEngine.exe") != 0 )
	{
		Global::connectEngine();
        if (ui.label_engine->text() != "成功连接到J3DEngine ")
		{
			ui.textBrowser->insertPlainText("与J3DEngine成功建立连接\n");
			QPalette pa;
			pa.setColor(QPalette::WindowText, Qt::green);
			ui.label_engine->setPalette(pa);
            ui.label_engine->setText("成功连接到J3DEngine ");
		}	
	}
	else
	{
        if (ui.label_engine->text() != "未连接到J3DEngine ")
		{
			ui.textBrowser->insertPlainText("与J3DEngine失去连接，之后将重新尝试连接\n");
			QPalette pa;
			pa.setColor(QPalette::WindowText, Qt::red);
			ui.label_engine->setPalette(pa);
            ui.label_engine->setText("未连接到J3DEngine ");
		}
	}
	

	if (Global::tasking == true)
	{
		Global::getProcessMsg();
		qDebug("%d %d %d", Global::process, Global::processProject, Global::processState);
		if (Global::process == PROCESSCLOSE)
		{
			ifstream cmdCache;
			cmdCache.open(("C:\\ProgramData\\J3DEngine\\cmdCache.tmp"), ios::in | ios::_Nocreate);
			std::string temp;
			getline(cmdCache, temp);
		if(temp == "sfmandsfp")
		{
			QString fileName = Global::sfmOutputDir + "/SparseCloud.j3d";
			if (fileName == "")
			{
				QMessageBox::information(NULL, "失败", "打开j3d文件失败，请检查路径是否正确 ", QMessageBox::Ok, QMessageBox::Ok);
				return;

			}
			openJ3DView(fileName);
		}
		else if (temp == "densifypointcloud") {
				QString fileName = Global::densifyWorkingDir + "/DenseCloud.j3d";
				if (fileName == "")
				{
					QMessageBox::information(NULL, "失败", "打开j3d文件失败，请检查路径是否正确 ", QMessageBox::Ok, QMessageBox::Ok);
					return;

				}
				openJ3DView(fileName);
			}
			else if (temp == "reconstructmesh") {
				QString fileName = Global::reconstructMeshWorkingDir + "/TIN_Mesh.j3d";
				if (fileName == "")
				{
					QMessageBox::information(NULL, "失败", "打开j3d文件失败，请检查路径是否正确 ", QMessageBox::Ok, QMessageBox::Ok);
					return;

				}
				openJ3DView(fileName);
			}
			else if (temp == "texturemesh") {
				QString fileName = Global::reconstructMeshWorkingDir + "/TEXTURE_Mesh.j3d";
				if (fileName == "")
				{
					QMessageBox::information(NULL, "失败", "打开j3d文件失败，请检查路径是否正确 ", QMessageBox::Ok, QMessageBox::Ok);
					return;

				}
				openJ3DView(fileName);
			}
            QMessageBox::information(NULL, "完成", "任务完成！ ", QMessageBox::Ok, QMessageBox::Ok);
			Global::tasking = false;
			ui.label_process->setText("等待任务 ");
			ui.progressBar->setValue(0);
			return;
		}
		else if (Global::process == PROCESSERROR)
		{
            QMessageBox::information(NULL, "失败", "任务失败，具体任务日志于J3DEngine查询 ", QMessageBox::Ok, QMessageBox::Ok);
			Global::tasking = false;
            ui.label_process->setText("等待任务 ");
			ui.progressBar->setValue(0);
			return;
		}
		else if (Global::processProject < 1)
		{
            ui.label_process->setText("等待任务 ");
			ui.progressBar->setValue(0);
			return;
		}
		else
		{

			switch (Global::processProject)
			{
			case LISTIMAGES:
			{
                ui.label_process->setText("任务进行中：加载图片 ");
				ui.progressBar->setValue(Global::processState);
				break;
			}
			case COMPUTEFEATURES:
			{
                ui.label_process->setText("任务进行中：计算特征点 ");
				ui.progressBar->setValue(Global::processState);
				break;
			}
			case MATCHFEATURES:
			{
                ui.label_process->setText("任务进行中：匹配特征点 ");
				ui.progressBar->setValue(Global::processState);
				break;
			}
			case SFM:
			{
                ui.label_process->setText("任务进行中：三维重建_SFM ");
				ui.progressBar->setValue(Global::processState);
				break;
			}
			case SFP:
			{
                ui.label_process->setText("任务进行中：三维重建_SFP ");
				ui.progressBar->setValue(Global::processState);
				break;
			}
			case COLORED:
			{
                ui.label_process->setText("任务进行中：点云上色 ");
				ui.progressBar->setValue(Global::processState);
				break;
			}
			case SPARSE:
			{
                ui.label_process->setText("任务进行中：生成稀疏点云 ");
				ui.progressBar->setValue(Global::processState);
				break;
			}
			case DENSE:
			{
                ui.label_process->setText("任务进行中：生成密集点云 ");
				ui.progressBar->setValue(Global::processState);
				break;
			}
			case REMESH:
			{
				ui.label_process->setText("任务进行中：生成三角网模型 ");
				ui.progressBar->setValue(Global::processState);
				break;
			}
			case REFINE:
			{
				ui.label_process->setText("任务进行中：优化三角网模型 ");
				ui.progressBar->setValue(Global::processState);
				break;
			}
			case TEXTURE:
			{
				ui.label_process->setText("任务进行中：生成纹理映射 ");
				ui.progressBar->setValue(Global::processState);
				break;
			}
			default:
			{
                ui.label_process->setText("等待任务 ");
				ui.progressBar->setValue(0);
				break;
			}
				
			}
			
			
		}
	}
}


void QT3DReconstruction::on_action_triggered() //textureMesh
{
	dlgtm.exec();
}

void QT3DReconstruction::on_actionopen_mvs_file_triggered()
{
	QString fileName = QFileDialog::getOpenFileName(NULL, "ViewJ3D", ".", "*.j3d");
	if (fileName == "") 
	{
		QMessageBox::information(NULL, "失败", "打开j3d文件失败，请检查路径是否正确 ", QMessageBox::Ok, QMessageBox::Ok);
		return;

	}
	openJ3DView(fileName);
	
}

bool QT3DReconstruction::openJ3DView(QString fileName)
{
	delete this->ui.widget;
	QString cmd = "J3DView.dll -k 2324 -i " + fileName;
	STARTUPINFO si = { sizeof(si) };
	PROCESS_INFORMATION pi;
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = true;
	QFile Processcache("C:\\ProgramData\\J3DEngine\\ViewerCache.tmp");
	if (!Processcache.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
	{
		QMessageBox::information(NULL, "失败", "打开缓存文件失败，请检查权限    ", QMessageBox::Ok, QMessageBox::Ok);
		return false;
	}
	Processcache.write("1");
	Processcache.close();
	if (!CreateProcess(
		NULL,
		(LPWSTR)cmd.toStdWString().c_str(),
		NULL,
		NULL,
		FALSE,
		CREATE_NEW_CONSOLE,
		NULL,
		NULL, &si, &pi))
	{
		QMessageBox::information(NULL, "失败", "打开j3d文件失败，Viewer程序文件不完整 ", QMessageBox::Ok, QMessageBox::Ok);
		return false;
	}
	time_t tm = time(NULL);
	while (!Global::CheckViewerMsg()) {
		if (time(NULL) - tm > 30) {
			QMessageBox::information(NULL, "失败", "打开j3d文件失败，请尝试用管理员身份运行软件 ", QMessageBox::Ok, QMessageBox::Ok);
			return false;
		}
	}
	this->ui.widget = new mvsviewer(1,this->ui.centralWidget);
	this->ui.widget->setObjectName(QString::fromUtf8("widget"));
	this->ui.widget->setGeometry(QRect(10, 70, 1361, 661));
	this->ui.widget->show();
	return true;
}
