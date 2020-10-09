/********************************************************************************
** Form generated from reading UI file 'QT3DReconstruction.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QT3DRECONSTRUCTION_H
#define UI_QT3DRECONSTRUCTION_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QWidget>
#include <j3dviewer.h>

QT_BEGIN_NAMESPACE

class Ui_QT3DReconstructionClass
{
public:
    QAction *actionMatchFeature;
    QAction *actionSFM;
    QAction *actionDenseCloud;
    QAction *action_viewMVS;
    QAction *action_addSensorWidth;
    QAction *action_viewPLY;
    QAction *action_reconstrctMesh;
    QAction *action;
    QWidget *centralWidget;
    QTextBrowser *textBrowser;
    QLabel *label_2;
    QLabel *label_engine;
    QLabel *label;
    QLabel *label_process;
    QProgressBar *progressBar;
    j3dviewer *J3DViewerWidget;
    QStatusBar *statusBar;
    QMenuBar *menuBar;
    QMenu *menu_2;
    QMenu *menu_3;

    void setupUi(QMainWindow *QT3DReconstructionClass)
    {
        if (QT3DReconstructionClass->objectName().isEmpty())
            QT3DReconstructionClass->setObjectName(QString::fromUtf8("QT3DReconstructionClass"));
        QT3DReconstructionClass->resize(1383, 803);
        QT3DReconstructionClass->setMinimumSize(QSize(0, 0));
        QIcon icon;
        icon.addFile(QString::fromUtf8("J3D_GUI.ico"), QSize(), QIcon::Normal, QIcon::Off);
        QT3DReconstructionClass->setWindowIcon(icon);
        actionMatchFeature = new QAction(QT3DReconstructionClass);
        actionMatchFeature->setObjectName(QString::fromUtf8("actionMatchFeature"));
        actionSFM = new QAction(QT3DReconstructionClass);
        actionSFM->setObjectName(QString::fromUtf8("actionSFM"));
        actionDenseCloud = new QAction(QT3DReconstructionClass);
        actionDenseCloud->setObjectName(QString::fromUtf8("actionDenseCloud"));
        action_viewMVS = new QAction(QT3DReconstructionClass);
        action_viewMVS->setObjectName(QString::fromUtf8("action_viewMVS"));
        action_viewMVS->setVisible(false);
        action_addSensorWidth = new QAction(QT3DReconstructionClass);
        action_addSensorWidth->setObjectName(QString::fromUtf8("action_addSensorWidth"));
        action_viewPLY = new QAction(QT3DReconstructionClass);
        action_viewPLY->setObjectName(QString::fromUtf8("action_viewPLY"));
        action_reconstrctMesh = new QAction(QT3DReconstructionClass);
        action_reconstrctMesh->setObjectName(QString::fromUtf8("action_reconstrctMesh"));
        action = new QAction(QT3DReconstructionClass);
        action->setObjectName(QString::fromUtf8("action"));
        centralWidget = new QWidget(QT3DReconstructionClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        textBrowser = new QTextBrowser(centralWidget);
        textBrowser->setObjectName(QString::fromUtf8("textBrowser"));
        textBrowser->setGeometry(QRect(100, 10, 521, 21));
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(10, 10, 81, 16));
        label_engine = new QLabel(centralWidget);
        label_engine->setObjectName(QString::fromUtf8("label_engine"));
        label_engine->setGeometry(QRect(1070, 20, 291, 21));
        QFont font;
        font.setFamily(QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221"));
        font.setPointSize(12);
        label_engine->setFont(font);
        label_engine->setLayoutDirection(Qt::LeftToRight);
        label_engine->setAlignment(Qt::AlignCenter);
        label = new QLabel(centralWidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 720, 161, 21));
        label_process = new QLabel(centralWidget);
        label_process->setObjectName(QString::fromUtf8("label_process"));
        label_process->setGeometry(QRect(170, 720, 391, 31));
        label_process->setFont(font);
        label_process->setAlignment(Qt::AlignCenter);
        progressBar = new QProgressBar(centralWidget);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setGeometry(QRect(570, 730, 791, 23));
        progressBar->setValue(0);
        J3DViewerWidget = new j3dviewer(centralWidget);
        J3DViewerWidget->setObjectName(QString::fromUtf8("J3DViewerWidget"));
        J3DViewerWidget->setGeometry(QRect(20, 50, 1351, 661));
        QT3DReconstructionClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(QT3DReconstructionClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        QT3DReconstructionClass->setStatusBar(statusBar);
        menuBar = new QMenuBar(QT3DReconstructionClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1383, 23));
        menu_2 = new QMenu(menuBar);
        menu_2->setObjectName(QString::fromUtf8("menu_2"));
        menu_3 = new QMenu(menuBar);
        menu_3->setObjectName(QString::fromUtf8("menu_3"));
        QT3DReconstructionClass->setMenuBar(menuBar);

        menuBar->addAction(menu_3->menuAction());
        menuBar->addAction(menu_2->menuAction());
        menu_2->addAction(actionMatchFeature);
        menu_2->addAction(actionSFM);
        menu_2->addAction(actionDenseCloud);
        menu_2->addAction(action_reconstrctMesh);
        menu_2->addAction(action);
        menu_3->addAction(action_viewMVS);
        menu_3->addAction(action_viewPLY);
        menu_3->addAction(action_addSensorWidth);

        retranslateUi(QT3DReconstructionClass);

        QMetaObject::connectSlotsByName(QT3DReconstructionClass);
    } // setupUi

    void retranslateUi(QMainWindow *QT3DReconstructionClass)
    {
        QT3DReconstructionClass->setWindowTitle(QCoreApplication::translate("QT3DReconstructionClass", "J3DGUI", nullptr));
        actionMatchFeature->setText(QCoreApplication::translate("QT3DReconstructionClass", "\347\211\271\345\276\201\345\214\271\351\205\215", nullptr));
        actionSFM->setText(QCoreApplication::translate("QT3DReconstructionClass", "SFM", nullptr));
        actionDenseCloud->setText(QCoreApplication::translate("QT3DReconstructionClass", "\345\257\206\351\233\206\347\202\271\344\272\221\347\224\237\346\210\220", nullptr));
        action_viewMVS->setText(QCoreApplication::translate("QT3DReconstructionClass", "\351\242\204\350\247\210\347\250\200\347\226\217\347\202\271\344\272\221(MVS)", nullptr));
        action_addSensorWidth->setText(QCoreApplication::translate("QT3DReconstructionClass", "\346\267\273\345\212\240\347\233\270\346\234\272\344\274\240\346\204\237\345\231\250\345\217\202\346\225\260", nullptr));
        action_viewPLY->setText(QCoreApplication::translate("QT3DReconstructionClass", "\346\211\223\345\274\200PLY\346\226\207\344\273\266", nullptr));
        action_reconstrctMesh->setText(QCoreApplication::translate("QT3DReconstructionClass", "\344\270\211\350\247\222\347\275\221\346\250\241\345\236\213\351\207\215\345\273\272", nullptr));
        action->setText(QCoreApplication::translate("QT3DReconstructionClass", "\346\250\241\345\236\213\347\272\271\347\220\206\346\230\240\345\260\204", nullptr));
        label_2->setText(QCoreApplication::translate("QT3DReconstructionClass", "\350\276\223\345\207\272\346\227\245\345\277\227", nullptr));
        label_engine->setText(QCoreApplication::translate("QT3DReconstructionClass", "Engine", nullptr));
        label->setText(QCoreApplication::translate("QT3DReconstructionClass", "\345\275\223\345\211\215\347\250\213\345\272\217\350\277\220\350\241\214\347\212\266\345\206\265\357\274\232", nullptr));
        label_process->setText(QCoreApplication::translate("QT3DReconstructionClass", "\347\255\211\345\276\205\344\273\273\345\212\241", nullptr));
        menu_2->setTitle(QCoreApplication::translate("QT3DReconstructionClass", "\344\270\211\347\273\264\351\207\215\345\273\272", nullptr));
        menu_3->setTitle(QCoreApplication::translate("QT3DReconstructionClass", "\346\226\207\344\273\266", nullptr));
    } // retranslateUi

};

namespace Ui {
    class QT3DReconstructionClass: public Ui_QT3DReconstructionClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QT3DRECONSTRUCTION_H
