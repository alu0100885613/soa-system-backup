#ifndef BACKUPWINDOW_H
#define BACKUPWINDOW_H

#include <QMainWindow>

namespace Ui {
class BackupWindow;
}

class BackupWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit BackupWindow(QWidget *parent = 0);
    ~BackupWindow();

private:
    Ui::BackupWindow *ui;
};

#endif // BACKUPWINDOW_H
