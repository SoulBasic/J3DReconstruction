#pragma once
#include "PlyIO.h"
#include <QtWidgets/QMainWindow>
#include "ui_QT3DReconstruction.h"
#include <Windows.h>
#include "dialog_matchfeature.h"
#include "dialog_sfm.h"
#include "dialog_densifypointcloud.h"
#include "Dialog_ReconstructMesh.h"
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
#include <qopenglwidget.h>
#include <qtimer.h>
#include <qinputdialog.h>
#include <fstream>







class QT3DReconstruction : public QMainWindow
{
	Q_OBJECT

public:
	QT3DReconstruction(QWidget *parent = Q_NULLPTR);
	PlyIO*ply;
private slots:

    void on_actionMatchFeature_triggered();

    void on_actionSFM_triggered();

    LPCWSTR LPSTRtoLPCWSTR(char* szStr);


    void on_action_viewMVS_triggered();

    void on_action_addSensorWidth_triggered();

    void on_action_viewPLY_triggered();

    void on_actionDenseCloud_triggered();

	void timerSlot();




    void on_action_reconstrctMesh_triggered();

private:
	Ui::QT3DReconstructionClass ui;
	Dialog_MatchFeature dlgmf;
	Dialog_SFM dlgsfm;
	Dialog_DensifyPointCloud dlgdense;
	Dialog_ReconstructMesh dlgrm;
	
	








};
