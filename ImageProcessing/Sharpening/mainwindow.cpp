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

//Laplace
void MainWindow::on_ProcessButton3_clicked()
{
    if(!image.data)
        return;
    cv::Mat kernel = (cv::Mat_<int>(3, 3) << 0, -1, 0, -1, 4, -1, 0, -1, 0);
    cv::Mat result;
    filter2D(image, result, -1, kernel, cv::Point(-1, -1), 0, 0);
    QImage img = cvMat2QImage(result);
    ui->DisplayLabel->setPixmap(QPixmap::fromImage(img));
    ui->DisplayLabel->resize(ui->DisplayLabel->pixmap()->size());
}

//Sobel
void MainWindow::on_ProcessButton2_clicked()
{
    if(!image.data)
        return;
    cv::Mat kernel_x, kernel_y;
    kernel_x = (cv::Mat_<int>(3, 3) << -1,0,1,-2,0,2,-1,0,1);
    cv::Mat x;
    cv::filter2D(image, x, -1, kernel_x, cv::Point(-1, -1), 0, 0);
    kernel_y = (cv::Mat_<int>(3, 3) << -1,-2,-1,0,0,0,1,2,1);
    cv::Mat y;
    cv::filter2D(image, y, -1, kernel_y, cv::Point(-1, -1), 0, 0);
    cv::Mat result = x + y;
    QImage img = cvMat2QImage(result);
    ui->DisplayLabel->setPixmap(QPixmap::fromImage(img));
    ui->DisplayLabel->resize(ui->DisplayLabel->pixmap()->size());
}

//Robert
void MainWindow::on_ProcessButton1_clicked()
{
    if(!image.data)
        return;
    cv::Mat kernel_x, kernel_y;
    kernel_x = (cv::Mat_<int>(2,2)<<1,0,0,-1);
    cv::Mat x;
    cv::filter2D(image, x,-1,kernel_x, cv::Point(-1,-1),0,0);
    kernel_y = (cv::Mat_<int>(2, 2) << 0, 1, -1, 0);
    cv::Mat y;
    cv::filter2D(image, y, -1, kernel_y, cv::Point(-1, -1), 0, 0);
    cv::Mat result = x + y;
    QImage img = cvMat2QImage(result);
    ui->DisplayLabel->setPixmap(QPixmap::fromImage(img));
    ui->DisplayLabel->resize(ui->DisplayLabel->pixmap()->size());
}
