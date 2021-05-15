#ifndef DIALOG_POINTINFO_H
#define DIALOG_POINTINFO_H
#include <QDialog>
namespace Ui {
class Dialog_PointInfo;
}

class Dialog_PointInfo : public QDialog
{
    Q_OBJECT

public:
	explicit Dialog_PointInfo(QWidget *parent = nullptr);
    ~Dialog_PointInfo();
	void setX(int x);
	void setY(int y);
	void setXi(double x);
	void setYi(double y);
	void setZi(double z);
	void clearXYZi();
	void setP(void* p) { _p = p; }
private slots:
	void on_pushButton_OK_clicked();
	void on_pushButton_cancel_clicked();
private:
    Ui::Dialog_PointInfo *ui;
	void* _p;
};

#endif // DIALOG_POINTINFO_H
