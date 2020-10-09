/********************************************************************************
** Form generated from reading UI file 'dialog_reconstructmesh.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOG_RECONSTRUCTMESH_H
#define UI_DIALOG_RECONSTRUCTMESH_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_Dialog_ReconstructMesh
{
public:
    QLabel *label_18;
    QPushButton *btn_OK;
    QPushButton *btn_CANCEL;
    QLabel *label_16;
    QPushButton *pushButton_browseOutputDir;
    QPushButton *pushButton_browseInputDir;
    QLineEdit *lineEdit_OutputDir;
    QLabel *label_15;
    QLineEdit *lineEdit_inputDir;

    void setupUi(QDialog *Dialog_ReconstructMesh)
    {
        if (Dialog_ReconstructMesh->objectName().isEmpty())
            Dialog_ReconstructMesh->setObjectName(QString::fromUtf8("Dialog_ReconstructMesh"));
        Dialog_ReconstructMesh->resize(495, 195);
        label_18 = new QLabel(Dialog_ReconstructMesh);
        label_18->setObjectName(QString::fromUtf8("label_18"));
        label_18->setGeometry(QRect(10, 20, 171, 16));
        btn_OK = new QPushButton(Dialog_ReconstructMesh);
        btn_OK->setObjectName(QString::fromUtf8("btn_OK"));
        btn_OK->setGeometry(QRect(140, 140, 81, 31));
        btn_CANCEL = new QPushButton(Dialog_ReconstructMesh);
        btn_CANCEL->setObjectName(QString::fromUtf8("btn_CANCEL"));
        btn_CANCEL->setGeometry(QRect(270, 140, 81, 31));
        label_16 = new QLabel(Dialog_ReconstructMesh);
        label_16->setObjectName(QString::fromUtf8("label_16"));
        label_16->setGeometry(QRect(10, 90, 121, 21));
        pushButton_browseOutputDir = new QPushButton(Dialog_ReconstructMesh);
        pushButton_browseOutputDir->setObjectName(QString::fromUtf8("pushButton_browseOutputDir"));
        pushButton_browseOutputDir->setGeometry(QRect(430, 90, 41, 23));
        pushButton_browseInputDir = new QPushButton(Dialog_ReconstructMesh);
        pushButton_browseInputDir->setObjectName(QString::fromUtf8("pushButton_browseInputDir"));
        pushButton_browseInputDir->setGeometry(QRect(430, 50, 41, 23));
        lineEdit_OutputDir = new QLineEdit(Dialog_ReconstructMesh);
        lineEdit_OutputDir->setObjectName(QString::fromUtf8("lineEdit_OutputDir"));
        lineEdit_OutputDir->setGeometry(QRect(130, 90, 291, 20));
        label_15 = new QLabel(Dialog_ReconstructMesh);
        label_15->setObjectName(QString::fromUtf8("label_15"));
        label_15->setGeometry(QRect(10, 50, 121, 21));
        lineEdit_inputDir = new QLineEdit(Dialog_ReconstructMesh);
        lineEdit_inputDir->setObjectName(QString::fromUtf8("lineEdit_inputDir"));
        lineEdit_inputDir->setGeometry(QRect(130, 50, 291, 20));

        retranslateUi(Dialog_ReconstructMesh);

        QMetaObject::connectSlotsByName(Dialog_ReconstructMesh);
    } // setupUi

    void retranslateUi(QDialog *Dialog_ReconstructMesh)
    {
        Dialog_ReconstructMesh->setWindowTitle(QCoreApplication::translate("Dialog_ReconstructMesh", "\344\270\211\350\247\222\347\275\221\346\250\241\345\236\213\351\207\215\345\273\272", nullptr));
        label_18->setText(QCoreApplication::translate("Dialog_ReconstructMesh", "\344\270\211\350\247\222\347\275\221\351\207\215\345\273\272\345\217\202\346\225\260\350\256\276\347\275\256:", nullptr));
        btn_OK->setText(QCoreApplication::translate("Dialog_ReconstructMesh", "\347\241\256\345\256\232", nullptr));
        btn_CANCEL->setText(QCoreApplication::translate("Dialog_ReconstructMesh", "\345\217\226\346\266\210", nullptr));
        label_16->setText(QCoreApplication::translate("Dialog_ReconstructMesh", "\344\270\211\350\247\222\347\275\221\346\250\241\345\236\213\350\276\223\345\207\272\347\233\256\345\275\225:", nullptr));
        pushButton_browseOutputDir->setText(QCoreApplication::translate("Dialog_ReconstructMesh", "\346\265\217\350\247\210", nullptr));
        pushButton_browseInputDir->setText(QCoreApplication::translate("Dialog_ReconstructMesh", "\346\265\217\350\247\210", nullptr));
        lineEdit_OutputDir->setText(QString());
        label_15->setText(QCoreApplication::translate("Dialog_ReconstructMesh", "\345\257\206\351\233\206\347\202\271\344\272\221\346\226\207\344\273\266\347\233\256\345\275\225:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Dialog_ReconstructMesh: public Ui_Dialog_ReconstructMesh {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOG_RECONSTRUCTMESH_H
