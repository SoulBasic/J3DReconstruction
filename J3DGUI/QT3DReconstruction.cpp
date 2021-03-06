#include "QT3DReconstruction.h"



using namespace std;

QT3DReconstruction::QT3DReconstruction(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(timerSlot()));
	timer->start(500);
	setWindowFlags(windowFlags()& ~Qt::WindowMaximizeButtonHint);
	setFixedSize(this->width(), this->height());
	J3DViewer = nullptr;
	J3DViewerAva = false;
	this->setAttribute(Qt::WA_DeleteOnClose);
	
#ifdef SUPPORT_OSG
	ui.comboBox->addItem("osgb");
#endif
#ifdef SUPPORT_GLTF
	ui.comboBox->addItem("gltf");
#endif
	std::thread(&QT3DReconstruction::init_thread, this).detach();
}


QT3DReconstruction::~QT3DReconstruction()
{
	CloseWindow(FindWindowA("GLFW30", "J3D Viewer"));
	delete timer;
	delete J3DViewer;
}

void QT3DReconstruction::on_actionMatchFeature_triggered() { dlgmf.exec(); }

void QT3DReconstruction::on_actionSFM_triggered() { dlgsfm.exec(); }


void QT3DReconstruction::on_action_addSensorWidth_triggered() { dlgasw.exec(); }

void QT3DReconstruction::on_actionDenseCloud_triggered() { dlgdense.exec(); }

void QT3DReconstruction::on_action_reconstrctMesh_triggered() { dlgrm.exec(); }

void QT3DReconstruction::timerSlot()
{
	if (Global::GetProcessidFromName("J3DEngine.exe") != 0)
	{
		Global::connectEngine();
		if (ui.label_engine->text() != u8"成功连接到J3DEngine ")
		{
			QPalette pa;
			pa.setColor(QPalette::WindowText, Qt::green);
			ui.label_engine->setPalette(pa);
			ui.label_engine->setText(u8"成功连接到J3DEngine ");
		}
	}
	else
	{
		if (ui.label_engine->text() != u8"未连接到J3DEngine ")
		{
			QPalette pa;
			pa.setColor(QPalette::WindowText, Qt::red);
			ui.label_engine->setPalette(pa);
			ui.label_engine->setText(u8"未连接到J3DEngine ");
		}
	}


	if (Global::tasking)
	{
		Global::getProcessMsg();
		if (Global::process == PROCESSCLOSE)
		{
			ifstream cmdCache;
			cmdCache.open(("C:\\ProgramData\\J3DEngine\\cmdCache.tmp"), ios::in | ios::_Nocreate);
			std::string temp;
			getline(cmdCache, temp);
			QString fileName = "";
			if (temp == "sfmandsfp" || temp == "importfromblocksexchange")
			{
				fileName = Global::sfmOutputDir + "/SparseCloud.J3D";
				J3DFile = Jutil::SparseFileName(fileName.toStdString());
			}
			else if (temp == "densifypointcloud") 
			{
				fileName = Global::densifyWorkingDir + "/DenseCloud.J3D";
				J3DFile = Jutil::SparseFileName(fileName.toStdString());
			}
			else if (temp == "reconstructmesh") 
			{
				fileName = Global::reconstructMeshWorkingDir + "/TIN_Mesh_Refine.J3D";
				J3DFile = Jutil::SparseFileName(fileName.toStdString());
			}
			else if (temp == "texturemesh") 
			{
				fileName = Global::reconstructMeshWorkingDir + "/TEXTURE_Mesh.J3D";
				J3DFile = Jutil::SparseFileName(fileName.toStdString());
			}
			QMessageBox::information(this, u8"完成", u8"任务完成! ", QMessageBox::Ok, QMessageBox::Ok);
			if (!fileName.isEmpty())
			{
				if (J3DViewerAva)
				{
					auto codec = QTextCodec::codecForName("gb2312");
					J3DViewer->window.NewModel(codec->fromUnicode(fileName).toStdString());
				}
				else
				{
					openViewCompatibility(fileName);
				}
			}
			Global::tasking = false;
			ui.label_process->setText(u8"等待任务 ");
			ui.progressBar->setValue(0);
			return;
		}
		else if (Global::process == PROCESSERROR)
		{
			QMessageBox::information(NULL, u8"失败", u8"任务失败，具体任务日志于J3DEngine查询 ", QMessageBox::Ok, QMessageBox::Ok);
			Global::tasking = false;
			ui.label_process->setText(u8"等待任务 ");
			ui.progressBar->setValue(0);
			return;
		}
		else if (Global::processProject < 1)
		{
			ui.label_process->setText(u8"等待任务 ");
			ui.progressBar->setValue(0);
			return;
		}
		else
		{

			switch (Global::processProject)
			{
			case LISTIMAGES:
			{
				ui.label_process->setText(u8"任务进行中：加载图片 ");
				ui.progressBar->setValue(Global::processState);
				break;
			}
			case COMPUTEFEATURES:
			{
				ui.label_process->setText(u8"任务进行中：计算特征点 ");
				ui.progressBar->setValue(Global::processState);
				break;
			}
			case MATCHFEATURES:
			{
				ui.label_process->setText(u8"任务进行中：匹配特征点 ");
				ui.progressBar->setValue(Global::processState);
				break;
			}
			case SFM:
			{
				ui.label_process->setText(u8"任务进行中：三维重建_SFM ");
				ui.progressBar->setValue(Global::processState);
				break;
			}
			case SFP:
			{
				ui.label_process->setText(u8"任务进行中：三维重建_SFP ");
				ui.progressBar->setValue(Global::processState);
				break;
			}
			case COLORED:
			{
				ui.label_process->setText(u8"任务进行中：点云上色 ");
				ui.progressBar->setValue(Global::processState);
				break;
			}
			case SPARSECLOUD:
			{
				ui.label_process->setText(u8"任务进行中：生成稀疏点云 ");
				ui.progressBar->setValue(Global::processState);
				break;
			}
			case DENSE:
			{
				ui.label_process->setText(u8"任务进行中：生成密集点云 ");
				ui.progressBar->setValue(Global::processState);
				break;
			}
			case REMESH:
			{
				ui.label_process->setText(u8"任务进行中：生成三角网模型 ");
				ui.progressBar->setValue(Global::processState);
				break;
			}
			case REFINE:
			{
				ui.label_process->setText(u8"任务进行中：优化三角网模型 ");
				ui.progressBar->setValue(Global::processState);
				break;
			}
			case TEXTURE:
			{
				ui.label_process->setText(u8"任务进行中：生成纹理映射 ");
				ui.progressBar->setValue(Global::processState);
				break;
			}
			default:
			{
				ui.label_process->setText(u8"等待任务 ");
				ui.progressBar->setValue(0);
				break;
			}

			}
		}
	}
}


void QT3DReconstruction::on_action_triggered() { dlgtm.exec(); }//textureMesh

void QT3DReconstruction::on_actionopen_mvs_file_triggered()
{
	if (J3DViewer == nullptr)
	{
		openView("default.ply");
		J3DFile = Jutil::SparseFileName("default.ply");
		return;
	}
	QString fileName = QFileDialog::getOpenFileName(NULL, "ViewJ3D", ".",
		"J3D Model Format(*.J3D);;Stanford Polygon File Format(*.ply);;Alias Wavefront Object(*.obj);;All Files(*.*)");
	if (fileName == "") return;
	J3DFile = Jutil::SparseFileName(fileName.toStdString());
	if (J3DViewerAva == false)
	{
		openView(fileName);
		return;
	}
	auto codec = QTextCodec::codecForName("gb2312");
	J3DViewer->window.NewModel(codec->fromUnicode(fileName).toStdString());
}

bool QT3DReconstruction::openView(QString fileName)
{
	J3DViewer = new VIEWER::Scene();
	// create viewer
	if (!J3DViewer->Init(1361, 661, _T("J3D Viewer"),
		fileName.toStdString().c_str(),
		NULL))
		return false;
	////置子窗口
	delete this->ui.widget;
	ui.widget = new mvsviewer(1, this->ui.centralWidget);
	ui.widget->setObjectName(QString::fromUtf8("widget"));
	ui.widget->setGeometry(QRect(10, 70, 1361, 661));
	ui.widget->show();
	ui.widget->update();
	J3DViewer->window.SetVisible(true);
	
	// enter viewer loop
	J3DViewerAva = true;
	J3DViewer->Loop();
	return true;
}

void QT3DReconstruction::on_action_fullauto_triggered() { dlgfa.exec(); }

void QT3DReconstruction::on_action_2_triggered()
{
	QString fileName = QFileDialog::getOpenFileName(NULL,
		u8"打开模型文件", ".",
		"J3D Model Format(*.J3D);;Stanford Polygon File Format(*.ply);;Alias Wavefront Object(*.obj);;OpenSceneGraph(*.osg);;OpenSceneGraph Binary(*.osgb);;All Files(*.*)");
	if (fileName == "") return;
	openViewCompatibility(fileName);
}


bool QT3DReconstruction::openViewCompatibility(QString fileName)
{
	string ext = Jutil::getExtentionName(fileName.toStdString());
	if ("osgb" == ext || "osg" == ext)
	{
		QString cmd = "OSGViewer.exe " + fileName;
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
			QMessageBox::information(NULL, u8"失败", u8"打开文件失败，找不到OSGViewer ", QMessageBox::Ok, QMessageBox::Ok);
			return false;
		}
	}
	else
	{
		QString cmd = "J3DViewer.exe " + fileName;
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
			QMessageBox::information(NULL, u8"失败", u8"打开文件失败，J3D Viewer程序文件不完整 ", QMessageBox::Ok, QMessageBox::Ok);
			return false;
		}
	}
	return true;
}



void QT3DReconstruction::closeEvent(QCloseEvent *event)
{
	WinExec("taskkill /f /im OSGViewer.exe", SW_HIDE);
	WinExec("taskkill /f /im J3DViewer.exe", SW_HIDE);
	if (Global::intersecting)
	{
		if (QMessageBox::Yes == QMessageBox::warning(&dlgci, u8"退出坐标映射", QString(u8"关闭窗口后已加载以及未保存的数据都会丢失,您确认要关闭窗口吗?"), QMessageBox::Yes | QMessageBox::No))
		{
			event->accept();

		}
		else
		{
			event->ignore();
			return;
		}
	}
	QMainWindow::closeEvent(event);
}

void QT3DReconstruction::on_pushButton_camera_clicked()
{
	if (!J3DViewerAva)return;
	J3DViewer->window.Key(GLFW_KEY_C, NULL, GLFW_RELEASE, 0);
}

void QT3DReconstruction::on_pushButton_pointcloud_clicked()
{
	if (!J3DViewerAva)return;
	J3DViewer->window.Key(GLFW_KEY_P, NULL, GLFW_RELEASE, 0);
}

void QT3DReconstruction::on_pushButton_mesh_clicked()
{
	if (!J3DViewerAva)return;
	J3DViewer->window.Key(GLFW_KEY_M, NULL, GLFW_RELEASE, 0);
}

void QT3DReconstruction::on_pushButton_texture_clicked()
{
	if (!J3DViewerAva)return;
	J3DViewer->window.Key(GLFW_KEY_T, NULL, GLFW_RELEASE, 0);
}

void QT3DReconstruction::on_pushButton_pointplus_clicked()
{
	if (!J3DViewerAva)return;
	J3DViewer->window.Key(GLFW_KEY_UP, NULL, GLFW_RELEASE, 0);
}

void QT3DReconstruction::on_pushButton_pointsub_clicked()
{
	if (!J3DViewerAva)return;
	J3DViewer->window.Key(GLFW_KEY_DOWN, NULL, GLFW_RELEASE, 0);
}

void QT3DReconstruction::on_pushButton_pointnumplus_clicked()
{
	if (!J3DViewerAva)return;
	J3DViewer->window.Key(GLFW_KEY_DOWN, NULL, GLFW_RELEASE, GLFW_PRESS);
}

void QT3DReconstruction::on_pushButton_pointnumsub_clicked()
{
	if (!J3DViewerAva)return;
	J3DViewer->window.Key(GLFW_KEY_UP, NULL, GLFW_RELEASE, GLFW_PRESS);
}

void QT3DReconstruction::on_pushButton_viewportplus_clicked()
{
	if (!J3DViewerAva)return;
	J3DViewer->window.Key(GLFW_KEY_LEFT, NULL, GLFW_RELEASE, 0);
}

void QT3DReconstruction::on_pushButton_viewportsub_clicked()
{
	if (!J3DViewerAva)return;
	J3DViewer->window.Key(GLFW_KEY_RIGHT, NULL, GLFW_RELEASE, 0);
}

void QT3DReconstruction::on_pushButton_export_clicked()
{
	if (!J3DViewerAva)
	{
		QMessageBox::information(NULL, u8"失败", u8"请先加载J3D模型 ", QMessageBox::Ok, QMessageBox::Ok);
		return;
	}

	std::string type = "." + ui.comboBox->currentText().toStdString();
	bool isOsgb = false;
	bool isGltf = false;
	if (".osgb" == type)
	{
		type = ".obj";
		isOsgb = true;
	}
	else if (".gltf" == type)
	{
		type = ".obj";
		isGltf = true;
	}

	if (J3DFile.getDir().empty())
	{
		QMessageBox::information(NULL, u8"错误", u8"暂时无法输出成其他格式", QMessageBox::Ok, QMessageBox::Ok);
		return;
	}
	std::string temp = J3DFile.getDir() + "/" + J3DFile.getFrontName() + "_export" + type;
	J3DViewer->Export(temp.c_str(), type.c_str(), false, true);

	if (isOsgb)
	{
		std::string temp1 = J3DFile.getDir() + "/" + J3DFile.getFrontName() + "_export.osgb";
		converseType(temp.c_str(), temp1.c_str());
	}

	if (isGltf)
	{
		std::string temp1 = J3DFile.getDir() + "/" + J3DFile.getFrontName() + "_export.gltf";
		QString cmd = "obj2gltf -i " + QString(temp.c_str()) + " -o " + QString(temp1.c_str());
		::system(cmd.toStdString().c_str());
	}
	QMessageBox::information(NULL, u8"完成", u8"成功输出文件", QMessageBox::Ok, QMessageBox::Ok);
}



bool QT3DReconstruction::converseType(QString fileNameSrc, QString fileNameDes)
{
	QString cmd = "osgconv.exe " + fileNameSrc + " " + fileNameDes;
	::system(cmd.toStdString().c_str());
	return true;
}


void QT3DReconstruction::on_action_coor_triggered()
{
	Global::intersecting = true;
	dlgci.show();
}
void QT3DReconstruction::on_action_import_BlocksExchange_triggered()
{
	QString fileName = QFileDialog::getOpenFileName(NULL,
		u8"请选择附带完整相机姿态(外方位元素以旋转矩阵的形式保存)的SFM数据", ".",
		"Blocks Exchange XML(*.xml);;All Files(*.*)");
	if (fileName == "")
	{
		return;
	}
	string ext = Jutil::getExtentionName(fileName.toStdString());
	if ("xml" == ext)
	{
		if (Global::GetProcessidFromName("J3DEngine.exe") == 0)
		{
			QMessageBox::critical(this, u8"错误 ", u8"未找到J3DEngine进程 ", QMessageBox::Ok, QMessageBox::Ok);
			return;
		}
		else
			Global::connectEngine();

		Global::importWorkingDir = Jutil::getFileDir(fileName.toStdString()).c_str();
		QString fn = Jutil::getFileName(fileName.toStdString()).c_str();
		_mkdir("C:\\ProgramData\\J3DEngine");

		QFile cmdcache("C:\\ProgramData\\J3DEngine\\cmdCache.tmp");

		if (cmdcache.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
		{
			QString head = "importfromblocksexchange\n";
			cmdcache.write(head.toUtf8());
			cmdcache.write(fn.toUtf8());
			cmdcache.write("\n");
			cmdcache.write(Global::importWorkingDir.toUtf8());
			cmdcache.write("\n");
			cmdcache.close();
			QMessageBox::information(NULL, u8"完成", u8"任务提交成功! ", QMessageBox::Yes, NULL);
			PostThreadMessageA(Global::engineTid, CMD_IMPORTFROMBE, 0, 0);
			Global::tasking = true;
		}
		else
		{
			QMessageBox::information(NULL, u8"错误", u8"无法访问缓存文件，请检查权限，或使用管理员身份运行 ", QMessageBox::Yes, NULL);
		}
	}
}
