#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "utility.h"
#include <opencv2/core/softfloat.hpp>
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

void MainWindow::on_OpenImageButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), ".", tr("Image Files(*.png *jpg *bmp)"));
    image = cv::imread(fileName.toStdString());
    QImage img = cvMat2QImage(image);
    ui->DisplayLabel->setPixmap(QPixmap::fromImage(img));
    ui->DisplayLabel->resize(ui->DisplayLabel->pixmap()->size());
}

void MainWindow::on_ProcessButton3_clicked()
{
    if(!image.data)
        return;
    cv::Mat result;
    cv::GaussianBlur(image, result, cv::Size(3, 3), 0, 0);
    QImage img = cvMat2QImage(result);
    ui->DisplayLabel->setPixmap(QPixmap::fromImage(img));
    ui->DisplayLabel->resize(ui->DisplayLabel->pixmap()->size());
}

void MainWindow::on_ProcessButton2_clicked()
{
    if(!image.data)
        return;
    cv::Mat result;
    cv::medianBlur(image, result, 7);
    QImage img = cvMat2QImage(result);
    ui->DisplayLabel->setPixmap(QPixmap::fromImage(img));
    ui->DisplayLabel->resize(ui->DisplayLabel->pixmap()->size());
}

void MainWindow::on_ProcessButton1_clicked()
{
    if(!image.data)
        return;
    cv::Mat result;
    cv::blur(image, result,cv::Size(3, 3));
    QImage img = cvMat2QImage(result);
    ui->DisplayLabel->setPixmap(QPixmap::fromImage(img));
    ui->DisplayLabel->resize(ui->DisplayLabel->pixmap()->size());
}
