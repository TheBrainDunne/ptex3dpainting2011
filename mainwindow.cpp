#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "glwidget.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    glview = new GLWidget(this);
    ui->setupUi(this);
    ui->gridLayout->addWidget(glview,0,0,1,1);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionBakePaint_triggered()
{
    this->glview->bakePaint();
}
