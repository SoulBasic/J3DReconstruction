#include "dialog_coorintersector.h"
#include "ui_dialog_coorintersector.h"

Dialog_CoorIntersector::Dialog_CoorIntersector(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_CoorIntersector)
{
    ui->setupUi(this);
}

Dialog_CoorIntersector::~Dialog_CoorIntersector()
{
    delete ui;
}
