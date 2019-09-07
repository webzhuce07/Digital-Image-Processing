#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "utility.h"
#include <QFileDialog>
#include <QDebug>

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
    image_ = cv::imread(fileName.toStdString());
    QImage img = cvMat2QImage(image_);
    ui->DisplayLabel->setPixmap(QPixmap::fromImage(img));
    ui->DisplayLabel->resize(ui->DisplayLabel->pixmap()->size());
}

void MainWindow::on_LogTransformButton_clicked()
{
    if(!image_.data)
        return;
    cv::Mat result = image_.clone();
    //log tranform
    double c = 1.0; //尺度比例常数
    double gray = 0;
    for (int i = 0; i < result.rows; i++) {
        for (int j = 0; j < result.cols; j++) {
            if(result.channels() == 1){
                gray = (double) result.at<uchar>(i, j);
                gray =  c * log(1.0 + gray);
                result.at<uchar>(i, j)= cv::saturate_cast<uchar>(gray);
            }
            else if(result.channels() == 3){
                for(int k = 0; k < 3; k++){
                    gray = (double)result.at<cv::Vec3b>(i, j)[k];
                    gray =  c * log((double)(1.5 + gray));
                    result.at<cv::Vec3b>(i, j)[k]= cv::saturate_cast<uchar>(gray);
                }
            }
        }
    }
    //归一化处理
    cv::normalize(result, result, 0, 255, cv::NORM_MINMAX);
    cv::convertScaleAbs(result, result);

    QImage img = cvMat2QImage(result);
    ui->DisplayLabel->setPixmap(QPixmap::fromImage(img));
    ui->DisplayLabel->resize(ui->DisplayLabel->pixmap()->size());
}

void MainWindow::on_GammaTransformButton_clicked()
{
    if(!image_.data)
        return;
    cv::Mat result = image_.clone();
    //gamma tranform
    double gamma = 0.4;  //伽马系数
    double comp = 0;     //补偿系数
    double gray = 0;
    for (int i = 0; i < result.rows; i++) {
        for (int j = 0; j < result.cols; j++) {
            if(result.channels() == 1){
                gray = (double) result.at<uchar>(i, j);
                gray =  pow((gray + comp) / 255.0, gamma) * 255.0;
                result.at<uchar>(i, j)= cv::saturate_cast<uchar>(gray);
            }
            else if(result.channels() == 3){
                for(int k = 0; k < 3; k++){
                    gray = (double)result.at<cv::Vec3b>(i, j)[k];
                    gray =  pow((gray + comp) / 255.0, gamma) * 255.0;
                    result.at<cv::Vec3b>(i, j)[k]= cv::saturate_cast<uchar>(gray);
                }
            }
        }
    }
    //归一化处理
    cv::normalize(result, result, 0, 255, cv::NORM_MINMAX);
    cv::convertScaleAbs(result, result);

    QImage img = cvMat2QImage(result);
    ui->DisplayLabel->setPixmap(QPixmap::fromImage(img));
    ui->DisplayLabel->resize(ui->DisplayLabel->pixmap()->size());
}
