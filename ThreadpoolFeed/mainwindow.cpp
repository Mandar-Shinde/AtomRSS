#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "webworker.h"
#include <qthreadpool.h>
#include <qdebug.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_Fetch_clicked()
{
    QUrl url(ui->lineEdit->text());
    WebWorker *w = new WebWorker(url);
    connect(w,SIGNAL(WorkCompleted(QString)),this,SLOT(DumpData(QString)));
    QThreadPool::globalInstance()->start(w);    
}

void MainWindow::DumpData(QString txt)
{
    ui->plainTextEdit->appendPlainText(txt);
}


