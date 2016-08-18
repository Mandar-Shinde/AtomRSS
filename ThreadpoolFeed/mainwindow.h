#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <webworker.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_Fetch_clicked();
    void DumpData(QString );

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
