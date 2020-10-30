#include "QT3DReconstruction.h"

using namespace std;
namespace OPT {
	String strInputFileName;
	String strOutputFileName;
	String strMeshFileName;
	bool bLosslessTexture;
	unsigned nArchiveType;
	int nProcessPriority;
	unsigned nMaxThreads;
	unsigned nMaxMemory;
	String strExportType;
	String strConfigFileName;
	String strCheckCode;
#if TD_VERBOSE != TD_VERBOSE_OFF
	bool bLogFile;
#endif
	boost::program_options::variables_map vm;
} // namespace OPT1
QT3DReconstruction::QT3DReconstruction(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	timer = new QTimer(this); //this 为parent类, 表示当前窗口

	connect(timer, SIGNAL(timeout()), this, SLOT(timerSlot()));

	timer->start(500); // 1000毫秒, 等于 1 秒
	setWindowFlags(windowFlags()& ~Qt::WindowMaximizeButtonHint);
	setFixedSize(this->width(), this->height());
	J3DViewer = nullptr;
	J3DViewerAva = false;
}


QT3DReconstruction::~QT3DReconstruction()
{
	CloseWindow(FindWindowA("GLFW30", "J3D Viewer"));
	delete timer;
	delete J3DViewer;
}


void QT3DReconstruction::on_actionMatchFeature_triggered()
{
	dlgmf.exec();
}

void QT3DReconstruction::on_actionSFM_triggered()
{
	dlgsfm.exec();
}




void QT3DReconstruction::on_action_viewMVS_triggered()
{

}

void QT3DReconstruction::on_action_addSensorWidth_triggered()
{
	dlgasw.exec();
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
		qDebug("%d %d %d", Global::process, Global::processProject, Global::processState);
		if (Global::process == PROCESSCLOSE)
		{
			ifstream cmdCache;
			cmdCache.open(("C:\\ProgramData\\J3DEngine\\cmdCache.tmp"), ios::in | ios::_Nocreate);
			std::string temp;
			getline(cmdCache, temp);
			if (temp == "sfmandsfp")
			{
				QString fileName = Global::sfmOutputDir + "/SparseCloud.J3D";
				if (fileName == "")
				{
					QMessageBox::information(NULL, u8"失败", "打开J3D文件失败，请检查路径是否正确 ", QMessageBox::Ok, QMessageBox::Ok);
					return;

				}
				if (J3DViewerAva)
				{
					const char* path[2];
					path[0] = fileName.toStdString().c_str();
					path[1] = NULL;
					J3DViewer->window.Drop(1, path);
				}
				else
				{
					openViewCompatibility(fileName);
				}

			}
			else if (temp == "densifypointcloud") {
				QString fileName = Global::densifyWorkingDir + "/DenseCloud.J3D";
				if (fileName == "")
				{
					QMessageBox::information(NULL, u8"失败", "打开J3D文件失败，请检查路径是否正确 ", QMessageBox::Ok, QMessageBox::Ok);
					return;

				}
				if (J3DViewerAva)
				{
					const char* path[2];
					path[0] = fileName.toStdString().c_str();
					path[1] = NULL;
					J3DViewer->window.Drop(1, path);
				}
				else
				{
					openViewCompatibility(fileName);
				}
			}
			else if (temp == "reconstructmesh") {
				QString fileName = Global::reconstructMeshWorkingDir + "/TIN_Mesh.J3D";
				if (fileName == "")
				{
					QMessageBox::information(NULL, u8"失败", "打开J3D文件失败，请检查路径是否正确 ", QMessageBox::Ok, QMessageBox::Ok);
					return;

				}
				if (J3DViewerAva)
				{
					const char* path[2];
					path[0] = fileName.toStdString().c_str();
					path[1] = NULL;
					J3DViewer->window.Drop(1, path);
				}
				else
				{
					openViewCompatibility(fileName);
				}
			}
			else if (temp == "texturemesh") {
				QString fileName = Global::reconstructMeshWorkingDir + "/TEXTURE_Mesh.J3D";
				if (fileName == "")
				{
					QMessageBox::information(NULL, u8"失败", "打开J3D文件失败，请检查路径是否正确 ", QMessageBox::Ok, QMessageBox::Ok);
					return;

				}
				if (J3DViewerAva)
				{
					const char* path[2];
					path[0] = fileName.toStdString().c_str();
					path[1] = NULL;
					J3DViewer->window.Drop(1, path);
				}
				else
				{
					openViewCompatibility(fileName);
				}
			}
			QMessageBox::information(NULL, u8"完成", u8"任务完成！ ", QMessageBox::Ok, QMessageBox::Ok);
			Global::tasking = false;
			ui.label_process->setText(u8"等待任务 ");
			ui.progressBar->setValue(0);
			return;
		}
		else if (Global::process == PROCESSERROR)
		{
			QMessageBox::information(NULL, u8"失败", "任务失败，具体任务日志于J3DEngine查询 ", QMessageBox::Ok, QMessageBox::Ok);
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


void QT3DReconstruction::on_action_triggered() //textureMesh
{
	dlgtm.exec();
}

void QT3DReconstruction::on_actionopen_mvs_file_triggered()
{
	QString fileName = QFileDialog::getOpenFileName(NULL, "ViewJ3D", ".",
		"J3D Model Format(*.J3D);;Stanford Polygon File Format(*.ply);;Alias Wavefront Object(*.obj);;All Files(*.*)");
	if (fileName == "")
	{
		QMessageBox::information(NULL, u8"失败", u8"打开J3D文件失败，请检查路径是否正确 ", QMessageBox::Ok, QMessageBox::Ok);
		return;
	}

	if (J3DViewerAva == false)
	{
		openView(fileName);
		return;
	}
	const char* path[2];
	path[0] = fileName.toStdString().c_str();
	path[1] = NULL;
	J3DViewer->window.Drop(1, path);
}



bool QT3DReconstruction::openView(QString fileName)
{
	LPCTSTR cmd[5];
	char t[200];
	GetModuleFileNameA(NULL, t, 200);
	cmd[0] = t;
	cmd[1] = "-k";
	cmd[2] = "2324";
	cmd[3] = "-i";
	cmd[4] = fileName.toStdString().c_str();
	LPCTSTR* l = cmd;

	if (!InitializeViewer(5, l))
		return false;

	J3DViewer = new VIEWER::Scene();
	// create viewer
	if (!J3DViewer->Init(1361, 661, _T("J3D Viewer"),
		fileName.toStdString().c_str(),
		NULL))
		return false;
	//if (viewer.IsOpen() && !OPT::strOutputFileName.IsEmpty()) {
	//	// export the scene
	//	viewer.Export(MAKE_PATH_SAFE(OPT::strOutputFileName), OPT::strExportType.IsEmpty()?LPCTSTR(NULL):OPT::strExportType.c_str(), OPT::bLosslessTexture);
	//}

	//置子窗口
	Sleep(1000);
	delete this->ui.widget;
	this->ui.widget = new mvsviewer(1, this->ui.centralWidget);
	this->ui.widget->setObjectName(QString::fromUtf8("widget"));
	this->ui.widget->setGeometry(QRect(10, 70, 1361, 661));
	this->ui.widget->show();

	J3DViewer->window.SetVisible(true);
	// enter viewer loop
	J3DViewerAva = true;
	J3DFile = Jutil::SparseFileName(fileName.toStdString());
	J3DViewer->Loop();
	FinalizeViewer();
	return true;
}


// initialize and parse the command line parameters
bool QT3DReconstruction::InitializeViewer(size_t argc, LPCTSTR* argv)
{
	// initialize log and console
	//OPEN_LOG();
	//OPEN_LOGCONSOLE();

	// group of options allowed only on command line
	boost::program_options::options_description generic("Generic options");
	generic.add_options()
		("help,h", "produce this help message")
		("working-folder,w", boost::program_options::value<std::string>(&WORKING_FOLDER), "working directory (default current directory)")
		("config-file,c", boost::program_options::value<std::string>(&OPT::strConfigFileName)->default_value(_T("J3D Viewer") _T(".cfg")), "file name containing program options")
		("check-code,k", boost::program_options::value<std::string>(&OPT::strCheckCode), "checkcode")
		("export-type", boost::program_options::value<std::string>(&OPT::strExportType), "file type used to export the 3D scene (ply or obj)")
		("archive-type", boost::program_options::value<unsigned>(&OPT::nArchiveType)->default_value(2), "project archive type: 0-text, 1-binary, 2-compressed binary")
		("process-priority", boost::program_options::value<int>(&OPT::nProcessPriority)->default_value(0), "process priority (normal by default)")
		("max-threads", boost::program_options::value<unsigned>(&OPT::nMaxThreads)->default_value(0), "maximum number of threads that this process should use (0 - use all available cores)")
		("max-memory", boost::program_options::value<unsigned>(&OPT::nMaxMemory)->default_value(0), "maximum amount of memory in MB that this process should use (0 - use all available memory)")
#if TD_VERBOSE != TD_VERBOSE_OFF
		("log-file", boost::program_options::value<bool>(&OPT::bLogFile)->default_value(false), "dump log to a file")
		("verbosity,v", boost::program_options::value<int>(&g_nVerbosityLevel)->default_value(
#if TD_VERBOSE == TD_VERBOSE_DEBUG
			3
#else
			2
#endif
		), "verbosity level")
#endif
		;


	// group of options allowed both on command line and in config file
	boost::program_options::options_description config("Viewer options");
	config.add_options()
		("input-file,i", boost::program_options::value<std::string>(&OPT::strInputFileName), "input project filename containing camera poses and scene (point-cloud/mesh)")
		("output-file,o", boost::program_options::value<std::string>(&OPT::strOutputFileName), "output filename for storing the mesh")
		("texture-lossless", boost::program_options::value<bool>(&OPT::bLosslessTexture)->default_value(false), "export texture using a lossless image format")
		;

	// hidden options, allowed both on command line and
	// in config file, but will not be shown to the user
	boost::program_options::options_description hidden("Hidden options");
	hidden.add_options()
		("mesh-file", boost::program_options::value<std::string>(&OPT::strMeshFileName), "mesh file name to texture (overwrite the existing mesh)")
		;

	boost::program_options::options_description cmdline_options;
	cmdline_options.add(generic).add(config).add(hidden);

	boost::program_options::options_description config_file_options;
	config_file_options.add(config).add(hidden);

	boost::program_options::positional_options_description p;
	p.add("input-file", -1);

	try {
		// parse command line options
		boost::program_options::store(boost::program_options::command_line_parser((int)argc, argv).options(cmdline_options).positional(p).run(), OPT::vm);
		boost::program_options::notify(OPT::vm);
		INIT_WORKING_FOLDER;
		// parse configuration file
		std::ifstream ifs(MAKE_PATH_SAFE(OPT::strConfigFileName));
		if (ifs) {
			boost::program_options::store(parse_config_file(ifs, config_file_options), OPT::vm);
			boost::program_options::notify(OPT::vm);
		}
	}
	catch (const std::exception& e) {
		LOG(e.what());
		return false;
	}

#if TD_VERBOSE != TD_VERBOSE_OFF
	// initialize the log file
	if (OPT::bLogFile)
		OPEN_LOGFILE((MAKE_PATH(_T("J3D Viewer") _T("-") + Util::getUniqueName(0) + _T(".log"))).c_str());
#endif
	//Priority check
	if (OPT::strCheckCode != "2324")
	{
		return false;
	}

	// print application details: version and command line
	//Util::LogBuild();
	//LOG(_T("Command line:%s"), Util::CommandLineToString(argc, argv).c_str());
	LOG(_T("J3D Viewer 初始化完成"));
	LOG(_T("读取文件中:%s"), Util::CommandLineToString(argc, argv).c_str());
	// validate input
	Util::ensureValidPath(OPT::strInputFileName);
	if (OPT::vm.count("help")) {
		boost::program_options::options_description visible("Available options");
		visible.add(generic).add(config);
		GET_LOG() << _T("\n"
			"Visualize any know point-cloud/mesh formats or MVS projects. Supply files through command line or Drag&Drop.\n"
			"Keys:\n"
			"\tE: export scene\n"
			"\tR: reset scene\n"
			"\tC: render cameras\n"
			"\tLeft/Right: select next camera to view the scene\n"
			"\tW: render wire-frame mesh\n"
			"\tT: render mesh texture\n"
			"\tUp/Down: adjust point size\n"
			"\tUp/Down + Shift: adjust minimum number of views accepted when displaying a point or line\n"
			"\t+/-: adjust camera thumbnail transparency\n"
			"\t+/- + Shift: adjust camera cones' length\n"
			"\n")
			<< visible;
	}
	if (!OPT::strExportType.IsEmpty())
		OPT::strExportType = OPT::strExportType.ToLower() == _T("obj") ? _T(".obj") : _T(".ply");

	// initialize optional options
	Util::ensureValidPath(OPT::strOutputFileName);
	Util::ensureValidPath(OPT::strMeshFileName);

	// initialize global options
	Process::setCurrentProcessPriority((Process::Priority)OPT::nProcessPriority);
#ifdef _USE_OPENMP
	if (OPT::nMaxThreads != 0)
		omp_set_num_threads(OPT::nMaxThreads);
#endif

#ifdef _USE_BREAKPAD
	// start memory dumper
	MiniDumper::Create(APPNAME, WORKING_FOLDER);
#endif

	Util::Init();
	return true;
}

// finalize application instance
void QT3DReconstruction::FinalizeViewer()
{
#if TD_VERBOSE != TD_VERBOSE_OFF
	// print memory statistics
	Util::LogMemoryInfo();
#endif

	if (OPT::bLogFile)
		CLOSE_LOGFILE();
	//CLOSE_LOGCONSOLE();
	//CLOSE_LOG();
}

void QT3DReconstruction::on_action_fullauto_triggered()
{
	dlgfa.exec();
}

void QT3DReconstruction::on_action_2_triggered()
{
	QString fileName = QFileDialog::getOpenFileName(NULL,
		"ViewJ3D", ".",
		"J3D Model Format(*.J3D);;Stanford Polygon File Format(*.ply);;Alias Wavefront Object(*.obj);;OpenSceneGraph(*.osg);;OpenSceneGraph Binary(*.osgb);;All Files(*.*)");
	if (fileName == "")
	{
		QMessageBox::information(NULL, u8"失败", u8"打开J3D文件失败，请检查路径是否正确 ", QMessageBox::Ok, QMessageBox::Ok);
		return;

	}
	string ext = Jutil::getExtentionName(fileName.toStdString());
	if ("osgb" == ext || "osg" == ext)
	{
		openViewCompatibility(fileName, true);
		return;
	}
	openViewCompatibility(fileName);
}

bool QT3DReconstruction::openViewCompatibility(QString fileName)
{
	QString cmd = "J3DView.dll -k 2324 -i " + fileName;
	STARTUPINFO si = { sizeof(si) };
	PROCESS_INFORMATION pi;
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = true;
	QFile Processcache("C:\\ProgramData\\J3DEngine\\ViewerCache.tmp");
	if (!Processcache.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
	{
		QMessageBox::information(NULL, u8"失败", u8"打开缓存文件失败，请检查权限    ", QMessageBox::Ok, QMessageBox::Ok);
		return false;
	}
	Processcache.write("1");
	Processcache.close();
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
		QMessageBox::information(NULL, u8"失败", u8"打开文件失败，Viewer程序文件不完整 ", QMessageBox::Ok, QMessageBox::Ok);
		return false;
	}
	time_t tm = time(NULL);
	while (!Global::CheckViewerMsg()) {
		if (time(NULL) - tm > 60) {
			QMessageBox::information(NULL, u8"失败", u8"打开文件失败，请尝试用管理员身份运行软件 ", QMessageBox::Ok, QMessageBox::Ok);
			WinExec("taskkill /f /im J3DView.dll", SW_HIDE);
			return false;
		}
	}
	return true;
}

bool QT3DReconstruction::openViewCompatibility(QString fileName, bool isOSG)
{
	if (false == isOSG)return false;

	QString cmd = "OSGView.dll " + fileName;
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
		QMessageBox::information(NULL, u8"失败", u8"打开文件失败，Viewer程序文件不完整 ", QMessageBox::Ok, QMessageBox::Ok);
		return false;
	}
	return true;

}
void QT3DReconstruction::closeEvent(QCloseEvent *event)
{
	//关闭时释放内存
	this->setAttribute(Qt::WA_DeleteOnClose);
	WinExec("taskkill /f /im OSGView.dll", SW_HIDE);
	WinExec("taskkill /f /im J3DView.dll", SW_HIDE);
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
	
	string type = "." + ui.comboBox->currentText().toStdString();
	bool isOsgb = false;
	if (".osgb" == type)
	{
		type = ".obj";
		isOsgb = true;
	}
	string temp = J3DFile.getDir() + "/"+ J3DFile.getFrontName() + "_export" + type;
	cout << "temp=" << temp << endl;
	cout << temp.c_str() << endl;
	J3DViewer->Export(temp.c_str(), type.c_str(), false, true);

	if (isOsgb)
	{
		string temp1 = J3DFile.getDir() + "/" + J3DFile.getFrontName() + "_export.osgb";
		converseType(temp.c_str(), temp1.c_str());
	}

	QMessageBox::information(NULL, u8"完成", u8"成功输出文件", QMessageBox::Ok, QMessageBox::Ok);
}


bool QT3DReconstruction::converseType(QString fileNameSrc, QString fileNameDes)
{
	QString cmd = "osgcv.dll " + fileNameSrc + " "+ fileNameDes;
	::system(cmd.toStdString().c_str());
	return true;
}