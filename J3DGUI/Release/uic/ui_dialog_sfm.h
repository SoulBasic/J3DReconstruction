/********************************************************************************
** Form generated from reading UI file 'dialog_sfm.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOG_SFM_H
#define UI_DIALOG_SFM_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_Dialog_SFM
{
public:
    QPushButton *pushButton_browseInputDir;
    QLabel *label_18;
    QLineEdit *lineEdit_inputDir;
    QLabel *label_15;
    QLabel *label_20;
    QLineEdit *lineEdit_OutputDir;
    QPushButton *pushButton_browseOutputDir;
    QLabel *label_21;
    QComboBox *comboBox_triangulationMethod;
    QComboBox *comboBox_resectionMethod;
    QLabel *label_22;
    QPushButton *btn_OK;
    QPushButton *btn_CANCEL;

    void setupUi(QDialog *Dialog_SFM)
    {
        if (Dialog_SFM->objectName().isEmpty())
            Dialog_SFM->setObjectName(QString::fromUtf8("Dialog_SFM"));
        Dialog_SFM->resize(491, 257);
        QIcon icon;
        icon.addFile(QString::fromUtf8("J3D_GUI.ico"), QSize(), QIcon::Normal, QIcon::Off);
        Dialog_SFM->setWindowIcon(icon);
        pushButton_browseInputDir = new QPushButton(Dialog_SFM);
        pushButton_browseInputDir->setObjectName(QString::fromUtf8("pushButton_browseInputDir"));
        pushButton_browseInputDir->setGeometry(QRect(430, 50, 41, 23));
        label_18 = new QLabel(Dialog_SFM);
        label_18->setObjectName(QString::fromUtf8("label_18"));
        label_18->setGeometry(QRect(10, 20, 171, 16));
        lineEdit_inputDir = new QLineEdit(Dialog_SFM);
        lineEdit_inputDir->setObjectName(QString::fromUtf8("lineEdit_inputDir"));
        lineEdit_inputDir->setGeometry(QRect(90, 50, 331, 20));
        label_15 = new QLabel(Dialog_SFM);
        label_15->setObjectName(QString::fromUtf8("label_15"));
        label_15->setGeometry(QRect(10, 50, 81, 21));
        label_20 = new QLabel(Dialog_SFM);
        label_20->setObjectName(QString::fromUtf8("label_20"));
        label_20->setGeometry(QRect(10, 80, 81, 21));
        lineEdit_OutputDir = new QLineEdit(Dialog_SFM);
        lineEdit_OutputDir->setObjectName(QString::fromUtf8("lineEdit_OutputDir"));
        lineEdit_OutputDir->setGeometry(QRect(90, 80, 331, 20));
        pushButton_browseOutputDir = new QPushButton(Dialog_SFM);
        pushButton_browseOutputDir->setObjectName(QString::fromUtf8("pushButton_browseOutputDir"));
        pushButton_browseOutputDir->setGeometry(QRect(430, 80, 41, 23));
        label_21 = new QLabel(Dialog_SFM);
        label_21->setObjectName(QString::fromUtf8("label_21"));
        label_21->setGeometry(QRect(10, 110, 111, 21));
        comboBox_triangulationMethod = new QComboBox(Dialog_SFM);
        comboBox_triangulationMethod->addItem(QString());
        comboBox_triangulationMethod->addItem(QString());
        comboBox_triangulationMethod->addItem(QString());
        comboBox_triangulationMethod->addItem(QString());
        comboBox_triangulationMethod->setObjectName(QString::fromUtf8("comboBox_triangulationMethod"));
        comboBox_triangulationMethod->setGeometry(QRect(120, 110, 231, 21));
        comboBox_resectionMethod = new QComboBox(Dialog_SFM);
        comboBox_resectionMethod->addItem(QString());
        comboBox_resectionMethod->addItem(QString());
        comboBox_resectionMethod->addItem(QString());
        comboBox_resectionMethod->addItem(QString());
        comboBox_resectionMethod->addItem(QString());
        comboBox_resectionMethod->setObjectName(QString::fromUtf8("comboBox_resectionMethod"));
        comboBox_resectionMethod->setGeometry(QRect(120, 140, 231, 21));
        label_22 = new QLabel(Dialog_SFM);
        label_22->setObjectName(QString::fromUtf8("label_22"));
        label_22->setGeometry(QRect(10, 140, 111, 21));
        btn_OK = new QPushButton(Dialog_SFM);
        btn_OK->setObjectName(QString::fromUtf8("btn_OK"));
        btn_OK->setGeometry(QRect(130, 200, 81, 31));
        btn_CANCEL = new QPushButton(Dialog_SFM);
        btn_CANCEL->setObjectName(QString::fromUtf8("btn_CANCEL"));
        btn_CANCEL->setGeometry(QRect(260, 200, 81, 31));

        retranslateUi(Dialog_SFM);

        comboBox_triangulationMethod->setCurrentIndex(3);
        comboBox_resectionMethod->setCurrentIndex(3);


        QMetaObject::connectSlotsByName(Dialog_SFM);
    } // setupUi

    void retranslateUi(QDialog *Dialog_SFM)
    {
        Dialog_SFM->setWindowTitle(QCoreApplication::translate("Dialog_SFM", "\344\270\211\347\273\264\351\207\215\345\273\272\351\205\215\347\275\256", nullptr));
        pushButton_browseInputDir->setText(QCoreApplication::translate("Dialog_SFM", "\346\265\217\350\247\210", nullptr));
        label_18->setText(QCoreApplication::translate("Dialog_SFM", "\344\270\211\347\273\264\351\207\215\345\273\272\345\217\202\346\225\260\350\256\276\347\275\256:", nullptr));
        label_15->setText(QCoreApplication::translate("Dialog_SFM", "\345\214\271\351\205\215\346\225\260\346\215\256\350\267\257\345\276\204:", nullptr));
        label_20->setText(QCoreApplication::translate("Dialog_SFM", "\347\273\223\346\236\234\350\276\223\345\207\272\350\267\257\345\276\204:", nullptr));
        pushButton_browseOutputDir->setText(QCoreApplication::translate("Dialog_SFM", "\346\265\217\350\247\210", nullptr));
        label_21->setText(QCoreApplication::translate("Dialog_SFM", "\347\251\272\344\270\255\344\270\211\350\247\222\346\265\213\351\207\217\346\226\271\346\263\225:", nullptr));
        comboBox_triangulationMethod->setItemText(0, QCoreApplication::translate("Dialog_SFM", "DIRECT_LINEAR_TRANSFORM", nullptr));
        comboBox_triangulationMethod->setItemText(1, QCoreApplication::translate("Dialog_SFM", "L1_ANGULAR", nullptr));
        comboBox_triangulationMethod->setItemText(2, QCoreApplication::translate("Dialog_SFM", "LINFINITY_ANGULAR", nullptr));
        comboBox_triangulationMethod->setItemText(3, QCoreApplication::translate("Dialog_SFM", "INVERSE_DEPTH_WEIGHTED_MIDPOINT", nullptr));

        comboBox_resectionMethod->setItemText(0, QCoreApplication::translate("Dialog_SFM", "DLT_6POINTS", nullptr));
        comboBox_resectionMethod->setItemText(1, QCoreApplication::translate("Dialog_SFM", "P3P_KE_CVPR17", nullptr));
        comboBox_resectionMethod->setItemText(2, QCoreApplication::translate("Dialog_SFM", "P3P_KNEIP_CVPR11", nullptr));
        comboBox_resectionMethod->setItemText(3, QCoreApplication::translate("Dialog_SFM", "P3P_NORDBERG_ECCV18", nullptr));
        comboBox_resectionMethod->setItemText(4, QCoreApplication::translate("Dialog_SFM", "UP2P_KUKELOVA_ACCV10", nullptr));

        label_22->setText(QCoreApplication::translate("Dialog_SFM", "\350\256\241\347\256\227\345\220\216\346\226\271\344\272\244\344\274\232\346\226\271\346\263\225:", nullptr));
        btn_OK->setText(QCoreApplication::translate("Dialog_SFM", "\347\241\256\345\256\232", nullptr));
        btn_CANCEL->setText(QCoreApplication::translate("Dialog_SFM", "\345\217\226\346\266\210", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Dialog_SFM: public Ui_Dialog_SFM {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOG_SFM_H
