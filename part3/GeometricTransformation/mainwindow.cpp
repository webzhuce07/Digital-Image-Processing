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

void MainWindow::on_OpenImageButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), ".", tr("Image Files(*.png *jpg *bmp)"));
    image = cv::imread(fileName.toStdString());
    QImage img = cvMat2QImage(image);
    ui->DisplayLabel->setPixmap(QPixmap::fromImage(img));
    ui->DisplayLabel->resize(ui->DisplayLabel->pixmap()->size());
}



void MainWindow::on_processButton2_clicked()
{
    if(!image.data)
        return;
    cv::Mat result = image.clone();
    cv::Point center = cv::Point(result.cols / 2, result.rows / 2);
    double angle = -50.0;
    double scale = 0.6;
    cv::Mat rotate_matrix( 2, 3, CV_32FC1 );
    rotate_matrix = cv::getRotationMatrix2D(center, angle, scale);
    cv::warpAffine(result, result, rotate_matrix, result.size());
    QImage img = cvMat2QImage(result);
    ui->DisplayLabel->setPixmap(QPixmap::fromImage(img));
    ui->DisplayLabel->resize(ui->DisplayLabel->pixmap()->size());
}

void MainWindow::on_processButton1_clicked()
{
    if(!image.data)
        return;
    cv::Mat result = image.clone();
    cv::Point2f srcTri[3];
    cv::Point2f dstTri[3];
    srcTri[0] = cv::Point2d(0.0, 0.0);
    srcTri[1] = cv::Point2d(2.0, 0.0);
    srcTri[2] = cv::Point2d(0.0, 1.0);

    dstTri[0] = cv::Point2d(10.0, 10.0);
    dstTri[1] = cv::Point2d(12.0, 10.0);
    dstTri[2] = cv::Point2d(10.0, 11.0);

    cv::Mat matrix( 2, 3, CV_32FC1 );
//    matrix.at<float>(0, 0) = 1;
//    matrix.at<float>(0, 1) = 0;
//    matrix.at<float>(0, 2) = 10;
//    matrix.at<float>(1, 0) = 0;
//    matrix.at<float>(1, 1) = 1;
//    matrix.at<float>(1, 2) = 10;
    matrix = cv::getAffineTransform(srcTri, dstTri);
    cv::warpAffine(result, result, matrix, result.size());
    QImage img = cvMat2QImage(result);
    ui->DisplayLabel->setPixmap(QPixmap::fromImage(img));
    ui->DisplayLabel->resize(ui->DisplayLabel->pixmap()->size());
}

