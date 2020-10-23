#pragma once
#include "PlyIO.h"
#include <QtWidgets/QMainWindow>
#include "ui_QT3DReconstruction.h"
#include <Windows.h>
#include "dialog_matchfeature.h"
#include "dialog_sfm.h"
#include "dialog_densifypointcloud.h"
#include "Dialog_ReconstructMesh.h"
#include "dialog_texturemesh.h"
#include "dialog_addsensorwidth.h"
#include "dialog_fullauto.h"
#include <iostream>
#include "qfiledialog.h"
#include "qmessagebox.h"
#include <qprocess.h>
#include <qdebug.h>
#include <qwindow.h>
#include <qwidget.h>
#include <qtextcodec.h>
#include "Global.h"
#include <qfile.h>
#include <qtimer.h>
#include <qinputdialog.h>
#include <fstream>
#include "MVSEngine.h"
#include "mvsviewer.h"
#include "qevent.h"
#include "Common.h"
#include <boost/program_options.hpp>
#include "Scene.h"
#include <time.h>

using namespace VIEWER;

class QT3DReconstruction : public QMainWindow
{
	Q_OBJECT

public:
	QT3DReconstruction(QWidget *parent = Q_NULLPTR);
	~QT3DReconstruction()
	{
		delete viewer;
	}

	bool openJ3DView(QString fileName);
private slots:

	void on_actionMatchFeature_triggered();

	void on_actionSFM_triggered();



	void on_action_viewMVS_triggered();

	void on_action_addSensorWidth_triggered();

	//void on_action_viewPLY_triggered();

	void on_actionDenseCloud_triggered();

	void timerSlot();




	void on_action_reconstrctMesh_triggered();

	void on_action_triggered();

	void on_actionopen_mvs_file_triggered();

	void on_action_fullauto_triggered();

private:

	Ui::QT3DReconstructionClass ui;
	Dialog_MatchFeature dlgmf;
	Dialog_SFM dlgsfm;
	Dialog_DensifyPointCloud dlgdense;
	Dialog_ReconstructMesh dlgrm;
	Dialog_TextureMesh dlgtm;
	Dialog_addsensorwidth dlgasw;
	Dialog_FullAuto dlgfa;
	bool InitializeViewer(size_t argc, LPCTSTR* argv);
	void FinalizeViewer();

	Scene* viewer;
protected:

};
