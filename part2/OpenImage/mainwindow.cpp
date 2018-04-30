#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "utility.h"
#include <QFileDialog>

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

void MainWindow::on_openImageButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), ".", tr("Image Files(*.png *jpg *bmp)"));
    image = cv::imread(fileName.toStdString());
    QImage img = cvMat2QImage(image);
    ui->displayLabel->setPixmap(QPixmap::fromImage(img));

}

void MainWindow::on_processButton_clicked()
{
    if(!image.data)
        return;
    cv::Mat result;
    image.copyTo(result);
    cv::flip(result, result, 1);
    QImage img = cvMat2QImage(result);
    ui->displayLabel->setPixmap(QPixmap::fromImage(img));
}
