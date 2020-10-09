/********************************************************************************
** Form generated from reading UI file 'dialog_densifypointcloud.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOG_DENSIFYPOINTCLOUD_H
#define UI_DIALOG_DENSIFYPOINTCLOUD_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_Dialog_DensifyPointCloud
{
public:
    QLineEdit *lineEdit_inputDir;
    QPushButton *pushButton_browseInputDir;
    QLabel *label_18;
    QLabel *label_15;
    QPushButton *pushButton_browseOutputDir;
    QLineEdit *lineEdit_OutputDir;
    QLabel *label_16;
    QPushButton *btn_OK;
    QPushButton *btn_CANCEL;

    void setupUi(QDialog *Dialog_DensifyPointCloud)
    {
        if (Dialog_DensifyPointCloud->objectName().isEmpty())
            Dialog_DensifyPointCloud->setObjectName(QString::fromUtf8("Dialog_DensifyPointCloud"));
        Dialog_DensifyPointCloud->resize(502, 189);
        lineEdit_inputDir = new QLineEdit(Dialog_DensifyPointCloud);
        lineEdit_inputDir->setObjectName(QString::fromUtf8("lineEdit_inputDir"));
        lineEdit_inputDir->setGeometry(QRect(130, 40, 301, 20));
        pushButton_browseInputDir = new QPushButton(Dialog_DensifyPointCloud);
        pushButton_browseInputDir->setObjectName(QString::fromUtf8("pushButton_browseInputDir"));
        pushButton_browseInputDir->setGeometry(QRect(440, 40, 51, 23));
        label_18 = new QLabel(Dialog_DensifyPointCloud);
        label_18->setObjectName(QString::fromUtf8("label_18"));
        label_18->setGeometry(QRect(10, 10, 171, 16));
        label_15 = new QLabel(Dialog_DensifyPointCloud);
        label_15->setObjectName(QString::fromUtf8("label_15"));
        label_15->setGeometry(QRect(10, 40, 121, 21));
        pushButton_browseOutputDir = new QPushButton(Dialog_DensifyPointCloud);
        pushButton_browseOutputDir->setObjectName(QString::fromUtf8("pushButton_browseOutputDir"));
        pushButton_browseOutputDir->setGeometry(QRect(440, 80, 51, 23));
        lineEdit_OutputDir = new QLineEdit(Dialog_DensifyPointCloud);
        lineEdit_OutputDir->setObjectName(QString::fromUtf8("lineEdit_OutputDir"));
        lineEdit_OutputDir->setGeometry(QRect(130, 80, 301, 20));
        label_16 = new QLabel(Dialog_DensifyPointCloud);
        label_16->setObjectName(QString::fromUtf8("label_16"));
        label_16->setGeometry(QRect(10, 80, 121, 21));
        btn_OK = new QPushButton(Dialog_DensifyPointCloud);
        btn_OK->setObjectName(QString::fromUtf8("btn_OK"));
        btn_OK->setGeometry(QRect(140, 130, 81, 31));
        btn_CANCEL = new QPushButton(Dialog_DensifyPointCloud);
        btn_CANCEL->setObjectName(QString::fromUtf8("btn_CANCEL"));
        btn_CANCEL->setGeometry(QRect(270, 130, 81, 31));

        retranslateUi(Dialog_DensifyPointCloud);

        QMetaObject::connectSlotsByName(Dialog_DensifyPointCloud);
    } // setupUi

    void retranslateUi(QDialog *Dialog_DensifyPointCloud)
    {
        Dialog_DensifyPointCloud->setWindowTitle(QCoreApplication::translate("Dialog_DensifyPointCloud", "\345\257\206\351\233\206\347\202\271\344\272\221\347\224\237\346\210\220", nullptr));
        pushButton_browseInputDir->setText(QCoreApplication::translate("Dialog_DensifyPointCloud", "\346\265\217\350\247\210", nullptr));
        label_18->setText(QCoreApplication::translate("Dialog_DensifyPointCloud", "\347\224\237\346\210\220\345\257\206\351\233\206\347\202\271\344\272\221\345\217\202\346\225\260\350\256\276\347\275\256:", nullptr));
        label_15->setText(QCoreApplication::translate("Dialog_DensifyPointCloud", "\344\270\211\347\273\264\351\207\215\345\273\272\347\273\223\346\236\234\347\233\256\345\275\225:", nullptr));
        pushButton_browseOutputDir->setText(QCoreApplication::translate("Dialog_DensifyPointCloud", "\346\265\217\350\247\210", nullptr));
        lineEdit_OutputDir->setText(QString());
        label_16->setText(QCoreApplication::translate("Dialog_DensifyPointCloud", "\345\257\206\351\233\206\347\202\271\344\272\221\350\276\223\345\207\272\347\233\256\345\275\225:", nullptr));
        btn_OK->setText(QCoreApplication::translate("Dialog_DensifyPointCloud", "\347\241\256\345\256\232", nullptr));
        btn_CANCEL->setText(QCoreApplication::translate("Dialog_DensifyPointCloud", "\345\217\226\346\266\210", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Dialog_DensifyPointCloud: public Ui_Dialog_DensifyPointCloud {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOG_DENSIFYPOINTCLOUD_H
