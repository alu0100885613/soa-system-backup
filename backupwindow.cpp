#include "backupwindow.h"
#include "ui_backupwindow.h"

BackupWindow::BackupWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BackupWindow)
{
    ui->setupUi(this);
}

BackupWindow::~BackupWindow()
{
    delete ui;
}
