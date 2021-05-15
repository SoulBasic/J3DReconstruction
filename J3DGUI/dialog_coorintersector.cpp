#include "dialog_coorintersector.h"
#include "ui_dialog_coorintersector.h"

#define NOW_TIME std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count()

Dialog_CoorIntersector::Dialog_CoorIntersector(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::Dialog_CoorIntersector),
	images_model(nullptr),
	picked_points_model(nullptr),
	scene(new MVS::Scene(0)),
	have_DSM_data(false),
	tiffData(new TiffData()),
	dlg_point_info(nullptr),
	picked_points_menu(nullptr)
{
	ui->setupUi(this);
	dlg_point_info.setP(this);
	ui->listView_picked_points->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(ui->listView_picked_points, SIGNAL(customContextMenuRequested(const QPoint&)),
		this, SLOT(show_picked_points_menu(const QPoint&)));
}

Dialog_CoorIntersector::~Dialog_CoorIntersector()
{
	delete images_model;
	delete picked_points_model;
	delete ui;
	delete tiffData;
	delete scene;
}

void Dialog_CoorIntersector::printMsg(const QString & msg)
{
	this->ui->label_msg->setText(msg);
	this->update();
}

void Dialog_CoorIntersector::onIntersection(int x, int y, int image_index, QPoint& now_pos)
{
	if (!have_DSM_data)
	{
		printMsg(u8"无有效DSM数据,请先加载");
		return;
	}
	dlg_point_info.setX(x);
	dlg_point_info.setY(y);
	dlg_point_info.clearXYZi();
	dlg_point_info.setGeometry(now_pos.x(), now_pos.y() - dlg_point_info.height(), dlg_point_info.width(), dlg_point_info.height());
	dlg_point_info.show();
	std::thread(&Dialog_CoorIntersector::intersectPoint, this, x, y, image_index).detach();
}

void Dialog_CoorIntersector::addPoint(double x, double y, double z)
{
	if (isnan(x) || isnan(y) || isnan(z))
	{
		printMsg(u8"添加点失败,点数据无效");
		return;
	}
	auto sl = picked_points_model->stringList();
	QString str;
	str.sprintf("%d: (%0.3lf, %0.3lf, %0.8lf)", sl.size() + 1, x, y, z);
	sl << str;
	picked_points_model->setStringList(sl);
	picked_points.push_back(SEACAVE::Point3d{ x,y,z });
	printMsg(u8"添加点成功");
}


void Dialog_CoorIntersector::intersectPoint(int x, int y, int image_index)
{
	time_t start_time = NOW_TIME;
	SEACAVE::Point3d p = tiffData->find(scene->images[image_index], x, y);
	dlg_point_info.setXi(p.x);
	dlg_point_info.setYi(p.y);
	dlg_point_info.setZi(p.z);
	if (isnan(p.x) || isnan(p.y) || isnan(p.z))
	{
		printMsg(u8"映射失败,鼠标处无有效点,用时:" + QString::number(NOW_TIME - start_time) + "ms");
	}
	else
	{
		printMsg(u8"映射成功,用时:" + QString::number(NOW_TIME - start_time) + "ms");
	}
}

void Dialog_CoorIntersector::on_toolButton_imageDir_clicked()
{
	workDir = QFileDialog::getExistingDirectory(this, u8"浏览重建路径 ", "", NULL);
	ui->lineEdit_imageDir->setText(workDir);
	if (!scene->IsEmpty())
	{
		delete scene;
		scene = new MVS::Scene(0);
	}

	std::thread(&Dialog_CoorIntersector::clear_sfm_file, this).detach();

	if (!scene->Load((workDir + "/SparseCloud.J3D").toStdString(), false))
	{
		QMessageBox::critical(this, u8"错误 ", u8"重建数据文件(SparseCloud.J3D)不存在，请检查路径", QMessageBox::Ok, QMessageBox::Ok);
		printMsg(u8"重建数据文件(SparseCloud.J3D)不存在，请检查路径");
		return;
	}
	printMsg(u8"读取完成,总共读取到 " + QString(to_string(scene->images.size()).c_str()).toUtf8() + u8" 组姿态数据");
	QStringList num;

	for (auto& i : scene->images)
	{
		num.append(Util::getFileNameExt(i.name.c_str()).c_str());
	}
	delete images_model;
	images_model = new QStringListModel(num);
	ui->listView_images->setModel(images_model);

	delete picked_points_model;
	picked_points_model = new QStringListModel(QStringList());
	ui->listView_picked_points->setModel(picked_points_model);

	QPalette pa1;
	pa1.setColor(QPalette::WindowText, Qt::green);
	ui->label_SFMinfo->setPalette(pa1);
	ui->label_SFMinfo->setText(u8"SFM数据可用，影像数量: " + QString(to_string(scene->images.size()).c_str()).toUtf8());

}
void Dialog_CoorIntersector::clear_sfm_file()
{
	QPalette pa;
	pa.setColor(QPalette::WindowText, Qt::black);
	ui->label_SFMinfo->setPalette(pa);
	ui->label_SFMinfo->setText(u8"无SFM重建数据，请先选择目录 ");
	printMsg(u8"正在读取SFM数据，请稍候... ");
}

void Dialog_CoorIntersector::on_toolButton_dsmDir_clicked()
{
	QString fileName = QFileDialog::getOpenFileName(NULL,
		u8"选择DSM文件", ".",
		"Tag Image File Format(*.tif);;All Files(*.*)");
	if (fileName == "")
	{
		return;
	}
	ui->lineEdit_dsmDir->setText(fileName);
	if (!tiffData->isEmpty())
	{
		delete tiffData;
		tiffData = new TiffData();
	}
	QPalette pa;
	pa.setColor(QPalette::WindowText, Qt::black);
	ui->label_DSMinfo->setPalette(pa);
	ui->label_DSMinfo->setText(u8"无DSM数据，请先选择文件 ");
	printMsg(u8"正在读取DSM数据，请稍候... ");
	std::thread(&Dialog_CoorIntersector::read_dsm_file, this, fileName).detach();
}

void Dialog_CoorIntersector::on_listView_images_clicked(QModelIndex index)
{
	ui->image_viewer->loadImage(scene->images[index.row()].name.c_str(), index.row());
}


void Dialog_CoorIntersector::show_picked_points_menu(const QPoint & pos)
{
	if (!((ui->listView_picked_points->selectionModel()->selectedIndexes()).empty()))
	{
		if (picked_points_menu != nullptr)delete picked_points_menu;
		picked_points_menu = new QMenu(this);
		QAction* deletePoint = picked_points_menu->addAction(u8"删除");
		connect(deletePoint, SIGNAL(triggered(bool)), this, SLOT(on_delete_point_clicked()));
		picked_points_menu->exec(QCursor::pos());//在当前鼠标位置显示
	}
}
void Dialog_CoorIntersector::on_delete_point_clicked()
{
	QModelIndex index = ui->listView_picked_points->currentIndex();
	picked_points.erase(picked_points.begin() + index.row());
	QStringList sl;
	for (int i = 0; i < picked_points.size(); i++)
	{
		auto& p = picked_points[i];
		QString str;
		str.sprintf("%d: (%lf,%lf,%lf)", i + 1, p.x, p.y, p.z);
		sl << str;
	}
	picked_points_model->setStringList(sl);
	printMsg(u8"删除点成功");
}
void Dialog_CoorIntersector::on_pushButton_outputDXF_clicked()
{
	QString fileName = QFileDialog::getSaveFileName(NULL,
		u8"选择输出位置", ".",
		"Autodesk Drawing Exchange Format(*.dxf);;All Files(*.*)");
	if (!write_to_dxf(fileName.toStdString()))
	{
		printMsg(u8"无法输出到目标路径，请检查路径或使用管理员权限重试");
	}
	else
	{
		printMsg(u8"输出到dxf文件成功!");
	}
}



void Dialog_CoorIntersector::read_dsm_file(QString fileName)
{
	QFile file(fileName);
	if (!file.exists())
	{
		printMsg(u8"文件不存在，请检查图片名称和路径");
		return;
	}
	file.close();
	if (!tiffData->read(fileName.toStdString()))
	{
		printMsg(u8"读取DSM数据失败:内存申请失败，tif文件过大");
		return;
	}
	printMsg(u8"读取DSM数据成功，Tiff图像 宽度：" + QString::number(tiffData->Width()) + u8" 高度：" + QString::number(tiffData->Height()));
	tiffData->set_origin_from_file((workDir + "/local_frame_origin.txt").toStdString());
	have_DSM_data = true;
	QPalette pa;
	pa.setColor(QPalette::WindowText, Qt::green);
	ui->label_DSMinfo->setPalette(pa);
	ui->label_DSMinfo->setText(u8"DSM数据可用,宽度: " + QString::number(tiffData->Width()) + u8" 高度:" + QString::number(tiffData->Height()));
}


bool Dialog_CoorIntersector::write_to_dxf(const std::string & fileName)
{
	DL_Dxf* dxf = new DL_Dxf();
	DL_Codes::version exportVersion = DL_Codes::AC1015;
	DL_WriterA* dw = dxf->out(fileName.c_str(), exportVersion);
	if (dw == NULL) 
	{
		return false;
	}
	dxf->writeHeader(*dw);
	dw->sectionEnd();
	dw->sectionTables();
	dxf->writeVPort(*dw);

	dw->tableLinetypes(3);
	dxf->writeLinetype(*dw, DL_LinetypeData("BYBLOCK", "BYBLOCK", 0, 0, 0.0));
	dxf->writeLinetype(*dw, DL_LinetypeData("BYLAYER", "BYLAYER", 0, 0, 0.0));
	dxf->writeLinetype(*dw, DL_LinetypeData("CONTINUOUS", "Continuous", 0, 0, 0.0));
	dw->tableEnd();

	int numberOfLayers = 3;
	dw->tableLayers(numberOfLayers);

	dxf->writeLayer(*dw,
		DL_LayerData("0", 0),
		DL_Attributes(
			std::string(""),      // leave empty
			DL_Codes::black,        // default color
			100,                  // default width
			"CONTINUOUS", 1.0));       // default line style

	dxf->writeLayer(*dw,
		DL_LayerData("mainlayer", 0),
		DL_Attributes(
			std::string(""),
			DL_Codes::white,
			100,
			"CONTINUOUS", 1.0));

	dxf->writeLayer(*dw,
		DL_LayerData("textlayer", 0),
		DL_Attributes(
			std::string(""),
			DL_Codes::white,
			100,
			"CONTINUOUS", 1.0));

	dw->tableEnd();

	dw->tableStyle(1);
	dxf->writeStyle(*dw, DL_StyleData("standard", 0, 2.5, 1.0, 0.0, 0, 2.5, "txt", ""));
	dw->tableEnd();

	dxf->writeView(*dw);
	dxf->writeUcs(*dw);

	dw->tableAppid(1);
	dxf->writeAppid(*dw, "ACAD");
	dw->tableEnd();

	dxf->writeDimStyle(*dw, 1, 1, 1, 1, 1);

	dxf->writeBlockRecord(*dw);
	dxf->writeBlockRecord(*dw, "myblock1");
	dxf->writeBlockRecord(*dw, "myblock2");
	dw->tableEnd();

	dw->sectionEnd();

	dw->sectionBlocks();
	dxf->writeBlock(*dw, DL_BlockData("*Model_Space", 0, 0.0, 0.0, 0.0));
	dxf->writeEndBlock(*dw, "*Model_Space");
	dxf->writeBlock(*dw, DL_BlockData("*Paper_Space", 0, 0.0, 0.0, 0.0));
	dxf->writeEndBlock(*dw, "*Paper_Space");
	dxf->writeBlock(*dw, DL_BlockData("*Paper_Space0", 0, 0.0, 0.0, 0.0));
	dxf->writeEndBlock(*dw, "*Paper_Space0");

	dxf->writeBlock(*dw, DL_BlockData("myblock1", 0, 0.0, 0.0, 0.0));

	dxf->writeEndBlock(*dw, "myblock1");

	dxf->writeBlock(*dw, DL_BlockData("myblock2", 0, 0.0, 0.0, 0.0));

	dxf->writeEndBlock(*dw, "myblock2");

	dw->sectionEnd();
	dw->sectionEntities();


	double sum = 0;
	int len = picked_points.size();
	for (int i = 0; i < len; i++)
		sum += picked_points[i].y;
	double avg = sum / len;
	sum = 0;
	for (int i = 0; i < len; i++)
		sum += pow(picked_points[i].y - avg, 2);
	double gap = sqrt(sum / len) / 20;
	for (int i = 0; i < picked_points.size(); i++)
	{
		auto& point = picked_points[i];
		dxf->writePoint(
			*dw,
			DL_PointData(point.x,
				point.y,
				point.z),
			DL_Attributes("mainlayer", 255, -1, "BYLAYER", 1.0));

		dxf->writeText(*dw, DL_TextData(point.x, point.y + gap, point.z,
			0.0, 0.0, 0.0,
			gap, 0,
			0, 0, 0,
			std::to_string(i + 1),
			"Standard",
			0),
			DL_Attributes("textlayer", 255, -1, "BYLAYER", 1.0)
		);
	}
	dw->sectionEnd();

	dxf->writeObjects(*dw);
	dxf->writeObjectsEnd(*dw);

	dw->dxfEOF();
	dw->close();
	delete dw;
	delete dxf;
	return true;
}
