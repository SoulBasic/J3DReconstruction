#ifndef DIALOG_COOR_H
#define DIALOG_COOR_H

#include <QDialog>
#include <string>
#include "libs/MVS/Common.h"
#include "libs/MVS/Scene.h"
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <qmessagebox.h>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <qstring.h>
#include <qmessagebox.h>
#include <qfiledialog.h>
#include "Global.h"
namespace Ui {
class dialog_coor;
}

class dialog_coor : public QDialog
{
    Q_OBJECT

public:
    explicit dialog_coor(QWidget *parent = nullptr);
    ~dialog_coor();

private slots:
	void on_button_ok_clicked();
	void on_pushButton_browseOutputDir_clicked();
	void on_pushButton_browseTiffDir_clicked();
private:
    Ui::dialog_coor *ui;
};

#endif // DIALOG_COOR_H
