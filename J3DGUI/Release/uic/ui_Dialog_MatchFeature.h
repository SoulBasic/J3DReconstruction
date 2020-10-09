/********************************************************************************
** Form generated from reading UI file 'Dialog_MatchFeature.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOG_MATCHFEATURE_H
#define UI_DIALOG_MATCHFEATURE_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_Dialog_MatchFeature
{
public:
    QLabel *label_7;
    QComboBox *comboBox_forcecompute;
    QComboBox *comboBox_upright;
    QLabel *label_8;
    QComboBox *comboBox_describer;
    QLabel *label_4;
    QLabel *label_2;
    QLabel *label_5;
    QComboBox *comboBox_quality;
    QLabel *label_3;
    QLabel *label;
    QLabel *label_6;
    QPushButton *btn_OK;
    QPushButton *btn_CANCEL;
    QComboBox *comboBox_geometricmodel;
    QLabel *label_9;
    QLabel *label_10;
    QComboBox *comboBox_forcematch;
    QLabel *label_11;
    QLabel *label_12;
    QLabel *label_13;
    QLineEdit *lineEdit_distanceRatio;
    QLabel *label_14;
    QLabel *label_15;
    QLineEdit *lineEdit_inputDir;
    QPushButton *pushButton_browseInputDir;
    QLabel *label_16;
    QLineEdit *lineEdit_kMatrix;
    QLabel *label_17;
    QLabel *label_18;
    QLabel *label_19;
    QLabel *label_20;
    QLineEdit *lineEdit_OutputDir;
    QPushButton *pushButton_browseOutputDir;

    void setupUi(QDialog *Dialog_MatchFeature)
    {
        if (Dialog_MatchFeature->objectName().isEmpty())
            Dialog_MatchFeature->setObjectName(QString::fromUtf8("Dialog_MatchFeature"));
        Dialog_MatchFeature->resize(507, 549);
        QIcon icon;
        icon.addFile(QString::fromUtf8("J3D_GUI.ico"), QSize(), QIcon::Normal, QIcon::Off);
        Dialog_MatchFeature->setWindowIcon(icon);
        label_7 = new QLabel(Dialog_MatchFeature);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(20, 200, 71, 21));
        comboBox_forcecompute = new QComboBox(Dialog_MatchFeature);
        comboBox_forcecompute->addItem(QString());
        comboBox_forcecompute->addItem(QString());
        comboBox_forcecompute->setObjectName(QString::fromUtf8("comboBox_forcecompute"));
        comboBox_forcecompute->setGeometry(QRect(130, 260, 61, 21));
        comboBox_upright = new QComboBox(Dialog_MatchFeature);
        comboBox_upright->addItem(QString());
        comboBox_upright->addItem(QString());
        comboBox_upright->setObjectName(QString::fromUtf8("comboBox_upright"));
        comboBox_upright->setGeometry(QRect(130, 230, 61, 21));
        label_8 = new QLabel(Dialog_MatchFeature);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(190, 200, 241, 21));
        comboBox_describer = new QComboBox(Dialog_MatchFeature);
        comboBox_describer->addItem(QString());
        comboBox_describer->addItem(QString());
        comboBox_describer->addItem(QString());
        comboBox_describer->addItem(QString());
        comboBox_describer->setObjectName(QString::fromUtf8("comboBox_describer"));
        comboBox_describer->setGeometry(QRect(90, 170, 201, 21));
        label_4 = new QLabel(Dialog_MatchFeature);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(190, 230, 161, 21));
        label_2 = new QLabel(Dialog_MatchFeature);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(20, 170, 71, 21));
        label_5 = new QLabel(Dialog_MatchFeature);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(20, 230, 111, 21));
        comboBox_quality = new QComboBox(Dialog_MatchFeature);
        comboBox_quality->addItem(QString());
        comboBox_quality->addItem(QString());
        comboBox_quality->addItem(QString());
        comboBox_quality->setObjectName(QString::fromUtf8("comboBox_quality"));
        comboBox_quality->setGeometry(QRect(90, 200, 101, 21));
        label_3 = new QLabel(Dialog_MatchFeature);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(20, 260, 101, 21));
        label = new QLabel(Dialog_MatchFeature);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(20, 140, 171, 16));
        label_6 = new QLabel(Dialog_MatchFeature);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(190, 260, 281, 21));
        btn_OK = new QPushButton(Dialog_MatchFeature);
        btn_OK->setObjectName(QString::fromUtf8("btn_OK"));
        btn_OK->setGeometry(QRect(130, 490, 81, 31));
        btn_CANCEL = new QPushButton(Dialog_MatchFeature);
        btn_CANCEL->setObjectName(QString::fromUtf8("btn_CANCEL"));
        btn_CANCEL->setGeometry(QRect(260, 490, 81, 31));
        comboBox_geometricmodel = new QComboBox(Dialog_MatchFeature);
        comboBox_geometricmodel->addItem(QString());
        comboBox_geometricmodel->addItem(QString());
        comboBox_geometricmodel->addItem(QString());
        comboBox_geometricmodel->addItem(QString());
        comboBox_geometricmodel->addItem(QString());
        comboBox_geometricmodel->addItem(QString());
        comboBox_geometricmodel->setObjectName(QString::fromUtf8("comboBox_geometricmodel"));
        comboBox_geometricmodel->setGeometry(QRect(90, 340, 291, 21));
        label_9 = new QLabel(Dialog_MatchFeature);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(20, 340, 71, 21));
        label_10 = new QLabel(Dialog_MatchFeature);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setGeometry(QRect(20, 310, 171, 16));
        comboBox_forcematch = new QComboBox(Dialog_MatchFeature);
        comboBox_forcematch->addItem(QString());
        comboBox_forcematch->addItem(QString());
        comboBox_forcematch->setObjectName(QString::fromUtf8("comboBox_forcematch"));
        comboBox_forcematch->setGeometry(QRect(90, 400, 61, 21));
        label_11 = new QLabel(Dialog_MatchFeature);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setGeometry(QRect(20, 400, 71, 21));
        label_12 = new QLabel(Dialog_MatchFeature);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setGeometry(QRect(160, 400, 271, 21));
        label_13 = new QLabel(Dialog_MatchFeature);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setGeometry(QRect(20, 370, 71, 21));
        lineEdit_distanceRatio = new QLineEdit(Dialog_MatchFeature);
        lineEdit_distanceRatio->setObjectName(QString::fromUtf8("lineEdit_distanceRatio"));
        lineEdit_distanceRatio->setGeometry(QRect(90, 370, 41, 20));
        label_14 = new QLabel(Dialog_MatchFeature);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setGeometry(QRect(130, 370, 381, 21));
        label_15 = new QLabel(Dialog_MatchFeature);
        label_15->setObjectName(QString::fromUtf8("label_15"));
        label_15->setGeometry(QRect(20, 50, 81, 21));
        lineEdit_inputDir = new QLineEdit(Dialog_MatchFeature);
        lineEdit_inputDir->setObjectName(QString::fromUtf8("lineEdit_inputDir"));
        lineEdit_inputDir->setGeometry(QRect(110, 50, 321, 20));
        pushButton_browseInputDir = new QPushButton(Dialog_MatchFeature);
        pushButton_browseInputDir->setObjectName(QString::fromUtf8("pushButton_browseInputDir"));
        pushButton_browseInputDir->setGeometry(QRect(440, 50, 41, 23));
        label_16 = new QLabel(Dialog_MatchFeature);
        label_16->setObjectName(QString::fromUtf8("label_16"));
        label_16->setGeometry(QRect(20, 80, 91, 21));
        lineEdit_kMatrix = new QLineEdit(Dialog_MatchFeature);
        lineEdit_kMatrix->setObjectName(QString::fromUtf8("lineEdit_kMatrix"));
        lineEdit_kMatrix->setGeometry(QRect(110, 80, 321, 20));
        label_17 = new QLabel(Dialog_MatchFeature);
        label_17->setObjectName(QString::fromUtf8("label_17"));
        label_17->setGeometry(QRect(440, 80, 61, 21));
        label_18 = new QLabel(Dialog_MatchFeature);
        label_18->setObjectName(QString::fromUtf8("label_18"));
        label_18->setGeometry(QRect(20, 20, 171, 16));
        label_19 = new QLabel(Dialog_MatchFeature);
        label_19->setObjectName(QString::fromUtf8("label_19"));
        label_19->setGeometry(QRect(110, 100, 331, 21));
        label_20 = new QLabel(Dialog_MatchFeature);
        label_20->setObjectName(QString::fromUtf8("label_20"));
        label_20->setGeometry(QRect(20, 450, 81, 21));
        lineEdit_OutputDir = new QLineEdit(Dialog_MatchFeature);
        lineEdit_OutputDir->setObjectName(QString::fromUtf8("lineEdit_OutputDir"));
        lineEdit_OutputDir->setGeometry(QRect(100, 450, 331, 20));
        pushButton_browseOutputDir = new QPushButton(Dialog_MatchFeature);
        pushButton_browseOutputDir->setObjectName(QString::fromUtf8("pushButton_browseOutputDir"));
        pushButton_browseOutputDir->setGeometry(QRect(440, 450, 41, 23));

        retranslateUi(Dialog_MatchFeature);
        QObject::connect(pushButton_browseInputDir, SIGNAL(clicked()), Dialog_MatchFeature, SLOT(pushButton_browseInputDir_clicked()));
        QObject::connect(pushButton_browseOutputDir, SIGNAL(clicked()), Dialog_MatchFeature, SLOT(pushButton_browseOutputDir_clicked()));

        comboBox_forcecompute->setCurrentIndex(0);
        comboBox_upright->setCurrentIndex(1);
        comboBox_describer->setCurrentIndex(0);
        comboBox_quality->setCurrentIndex(0);
        comboBox_geometricmodel->setCurrentIndex(0);
        comboBox_forcematch->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(Dialog_MatchFeature);
    } // setupUi

    void retranslateUi(QDialog *Dialog_MatchFeature)
    {
        Dialog_MatchFeature->setWindowTitle(QCoreApplication::translate("Dialog_MatchFeature", "\347\211\271\345\276\201\345\214\271\351\205\215\351\205\215\347\275\256", nullptr));
        label_7->setText(QCoreApplication::translate("Dialog_MatchFeature", "\347\211\271\345\276\201\347\202\271\350\264\250\351\207\217:", nullptr));
        comboBox_forcecompute->setItemText(0, QCoreApplication::translate("Dialog_MatchFeature", "\345\220\246", nullptr));
        comboBox_forcecompute->setItemText(1, QCoreApplication::translate("Dialog_MatchFeature", "\346\230\257", nullptr));

        comboBox_upright->setItemText(0, QCoreApplication::translate("Dialog_MatchFeature", "\345\220\246", nullptr));
        comboBox_upright->setItemText(1, QCoreApplication::translate("Dialog_MatchFeature", "\346\230\257", nullptr));

        label_8->setText(QCoreApplication::translate("Dialog_MatchFeature", "\357\274\210\346\236\201\351\253\230\345\223\201\350\264\250\345\217\257\350\203\275\344\274\232\350\212\261\350\264\271\351\235\236\345\270\270\351\225\277\347\232\204\346\227\266\351\227\264\357\274\211", nullptr));
        comboBox_describer->setItemText(0, QCoreApplication::translate("Dialog_MatchFeature", "SIFT", nullptr));
        comboBox_describer->setItemText(1, QCoreApplication::translate("Dialog_MatchFeature", "SIFT_ANATOMY", nullptr));
        comboBox_describer->setItemText(2, QCoreApplication::translate("Dialog_MatchFeature", "AKAZE_FLOAT", nullptr));
        comboBox_describer->setItemText(3, QCoreApplication::translate("Dialog_MatchFeature", "AKAZE_MLDB", nullptr));

        label_4->setText(QCoreApplication::translate("Dialog_MatchFeature", "\357\274\210\346\227\213\350\275\254\347\232\204\347\211\271\345\276\201\346\240\207\345\277\227\357\274\211", nullptr));
        label_2->setText(QCoreApplication::translate("Dialog_MatchFeature", "\347\211\271\345\276\201\347\202\271\347\256\227\345\255\220:", nullptr));
        label_5->setText(QCoreApplication::translate("Dialog_MatchFeature", "\350\256\241\347\256\227\347\211\271\345\276\201\347\251\272\351\227\264\345\220\221\351\207\217:", nullptr));
        comboBox_quality->setItemText(0, QCoreApplication::translate("Dialog_MatchFeature", "\351\273\230\350\256\244", nullptr));
        comboBox_quality->setItemText(1, QCoreApplication::translate("Dialog_MatchFeature", "\351\253\230\345\223\201\350\264\250", nullptr));
        comboBox_quality->setItemText(2, QCoreApplication::translate("Dialog_MatchFeature", "\346\236\201\351\253\230\345\223\201\350\264\250", nullptr));

        label_3->setText(QCoreApplication::translate("Dialog_MatchFeature", "\347\211\271\345\276\201\346\232\264\345\212\233\350\256\241\347\256\227:", nullptr));
        label->setText(QCoreApplication::translate("Dialog_MatchFeature", "\350\256\241\347\256\227\347\211\271\345\276\201\347\202\271\345\217\202\346\225\260\350\256\276\347\275\256:", nullptr));
        label_6->setText(QCoreApplication::translate("Dialog_MatchFeature", "\357\274\210\345\274\200\345\220\257\345\220\216\357\274\214\350\213\245\346\211\276\344\270\215\345\210\260\347\211\271\345\276\201\357\274\214\345\210\231\345\274\272\345\210\266\345\217\226\350\267\235\347\246\273\346\234\200\350\277\221\347\232\204\347\202\271\357\274\211", nullptr));
        btn_OK->setText(QCoreApplication::translate("Dialog_MatchFeature", "\347\241\256\345\256\232", nullptr));
        btn_CANCEL->setText(QCoreApplication::translate("Dialog_MatchFeature", "\345\217\226\346\266\210", nullptr));
        comboBox_geometricmodel->setItemText(0, QCoreApplication::translate("Dialog_MatchFeature", "fundamental matrix", nullptr));
        comboBox_geometricmodel->setItemText(1, QCoreApplication::translate("Dialog_MatchFeature", "esential matrix", nullptr));
        comboBox_geometricmodel->setItemText(2, QCoreApplication::translate("Dialog_MatchFeature", "homography matrix", nullptr));
        comboBox_geometricmodel->setItemText(3, QCoreApplication::translate("Dialog_MatchFeature", "essential matrix with an angular parametrization", nullptr));
        comboBox_geometricmodel->setItemText(4, QCoreApplication::translate("Dialog_MatchFeature", "orthographic essential matrix", nullptr));
        comboBox_geometricmodel->setItemText(5, QCoreApplication::translate("Dialog_MatchFeature", "upright essential matrix", nullptr));

        label_9->setText(QCoreApplication::translate("Dialog_MatchFeature", "\345\207\240\344\275\225\346\250\241\345\236\213:", nullptr));
        label_10->setText(QCoreApplication::translate("Dialog_MatchFeature", "\345\214\271\351\205\215\347\211\271\345\276\201\347\202\271\345\217\202\346\225\260\350\256\276\347\275\256:", nullptr));
        comboBox_forcematch->setItemText(0, QCoreApplication::translate("Dialog_MatchFeature", "\345\220\246", nullptr));
        comboBox_forcematch->setItemText(1, QCoreApplication::translate("Dialog_MatchFeature", "\346\230\257", nullptr));

        label_11->setText(QCoreApplication::translate("Dialog_MatchFeature", "\346\232\264\345\212\233\345\214\271\351\205\215:", nullptr));
        label_12->setText(QCoreApplication::translate("Dialog_MatchFeature", "\357\274\210\345\274\200\345\220\257\345\220\216\357\274\214\345\214\271\351\205\215\344\270\215\345\210\260\347\211\271\345\276\201\344\274\232\345\274\272\345\210\266\351\207\215\347\256\227\346\225\260\346\215\256\357\274\211", nullptr));
        label_13->setText(QCoreApplication::translate("Dialog_MatchFeature", "\350\267\235\347\246\273\346\257\224\347\216\207:", nullptr));
        lineEdit_distanceRatio->setText(QCoreApplication::translate("Dialog_MatchFeature", "0.8", nullptr));
        label_14->setText(QCoreApplication::translate("Dialog_MatchFeature", "\357\274\210\350\210\215\345\274\203\346\227\240\346\204\217\344\271\211\345\214\271\351\205\215\347\202\271\347\232\204\350\267\235\347\246\273\346\257\224\347\216\207\357\274\214\344\275\277\347\224\2500.6\346\210\226\346\233\264\344\275\216\346\257\224\347\216\207\345\217\257\344\273\245\345\207\217\345\260\221\350\257\257\346\212\245\357\274\211", nullptr));
        label_15->setText(QCoreApplication::translate("Dialog_MatchFeature", "\345\233\276\347\211\207\346\226\207\344\273\266\345\244\271:", nullptr));
        pushButton_browseInputDir->setText(QCoreApplication::translate("Dialog_MatchFeature", "\346\265\217\350\247\210", nullptr));
        label_16->setText(QCoreApplication::translate("Dialog_MatchFeature", "\347\233\270\346\234\272\346\234\254\345\276\201\347\237\251\351\230\265:", nullptr));
        lineEdit_kMatrix->setText(QString());
        label_17->setText(QCoreApplication::translate("Dialog_MatchFeature", "(\345\217\257\345\277\275\347\225\245)", nullptr));
        label_18->setText(QCoreApplication::translate("Dialog_MatchFeature", "\346\272\220\345\233\276\347\211\207\345\217\202\346\225\260\350\256\276\347\275\256:", nullptr));
        label_19->setText(QCoreApplication::translate("Dialog_MatchFeature", "\350\276\223\345\205\245\346\240\274\345\274\217: f;0;ppx;0;f;ppy;0;0;1", nullptr));
        label_20->setText(QCoreApplication::translate("Dialog_MatchFeature", "\347\273\223\346\236\234\350\276\223\345\207\272\350\267\257\345\276\204:", nullptr));
        pushButton_browseOutputDir->setText(QCoreApplication::translate("Dialog_MatchFeature", "\346\265\217\350\247\210", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Dialog_MatchFeature: public Ui_Dialog_MatchFeature {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOG_MATCHFEATURE_H
