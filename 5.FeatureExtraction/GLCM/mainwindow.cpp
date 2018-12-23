#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "utility.h"
#include "math.h"
#include <QFileDialog>
#include <QDebug>

using namespace cv;

MainWindow::MainWindow(QWidget *parentropy) :
    QMainWindow(parentropy),
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

void MainWindow::on_ProcessButton1_clicked()
{
    if(!image_.data)
        return;
    Mat result;
    Mat gray;
    cvtColor(image_, gray, CV_RGB2GRAY);
    getGLCM0(gray, result, 256);
    double energy, entropy, contrast, idMoment;
    featureGLCM(result, energy, entropy, contrast, idMoment);
    qDebug()<<"能量:" << energy <<endl;
    qDebug()<< "entropy:" << entropy << endl;
    qDebug()<< "对比度：" << contrast << endl;
    qDebug()<< "逆方差：" << idMoment << endl;

}

// 0°GLCM
void MainWindow::getGLCM0(cv::Mat& src, cv::Mat& dst, int gray_level)
{
    CV_Assert(1 == src.channels());
    int height = src.rows;
    int width = src.cols;
    dst.create(gray_level, gray_level, CV_32SC1);
    dst = Scalar::all(0);
    for (int i = 0; i < height; i++) {
        uchar* srcdata = src.ptr<uchar>(i);
        for (int j = 0; j < width - 1; j++){
            //horizontal
            int rows = srcdata[j];
            int cols = srcdata[j + 1];
            dst.ptr<int>(rows)[cols]++;
        }
    }
}

// 90°GLCM
void MainWindow::getGLCM90(cv::Mat& src, cv::Mat& dst, int gray_level)
{
    CV_Assert(1 == src.channels());
    int height = src.rows;
    int width = src.cols;
    dst = Mat(gray_level, gray_level, CV_32SC1, Scalar(0));
    for (int i = 0; i < height - 1; i++){
        uchar* srcdata = src.ptr<uchar>(i);
        uchar* srcdata1 = src.ptr<uchar>(i + 1);
        for (int j = 0; j < width; j++){
            // vertical
            int rows = srcdata[j];
            int cols = srcdata1[j];
            dst.ptr<int>(rows)[cols]++;
        }
    }
}

// 45°GLCM
void MainWindow::getGLCM45(cv::Mat& src, cv::Mat& dst, int gray_level)
{
    CV_Assert(1 == src.channels());
    int height = src.rows;
    int width = src.cols;
    dst = Mat(gray_level, gray_level, CV_32SC1, Scalar(0));
    for (int i = 0; i < height - 1; i++){
        uchar* srcdata = src.ptr<uchar>(i);
        uchar* srcdata1 = src.ptr<uchar>(i + 1);
        for (int j = 0; j < width - 1; j++) {
            // +45°
            int rows = srcdata[j];
            int cols = srcdata1[j + 1];
            dst.ptr<int>(rows)[cols]++;
        }
    }
}

// 135°GLCM
void MainWindow::getGLCM135(cv::Mat& src, cv::Mat& dst, int gray_level)
{
    CV_Assert(1 == src.channels());
    int height = src.rows;
    int width = src.cols;
    dst = Mat(gray_level, gray_level, CV_32SC1, Scalar(0));
    for (int i = 0; i < height - 1; i++) {
        uchar* srcdata = src.ptr<uchar>(i);
        uchar* srcdata1 = src.ptr<uchar>(i + 1);
        for (int j = 1; j < width; j++) {
            // -45°
            int rows = srcdata[j];
            int cols = srcdata1[j - 1];
            dst.ptr<int>(rows)[cols]++;
        }
    }

}

// computer features
void MainWindow::featureGLCM(cv::Mat&src, double& energy, double& entropy, double& contrast, double& idMoment)
{
    CV_Assert(src.channels() == 1);
    int height = src.rows;
    int width = src.cols;
    int total = 0;
    //computer sum of gray of every pixel
    for (int i = 0; i < height; i++) {
        int*srcdata = src.ptr<int>(i);
        for (int j = 0; j < width; j++)
            total += srcdata[j];
    }

    //every pixel / sum
    Mat mean;
    mean.create(height, width, CV_64FC1);
    for (int i = 0; i < height; i++) {
        int*srcdata = src.ptr<int>(i);
        double*copydata = mean.ptr<double>(i);
        for (int j = 0; j < width; j++)
            copydata[j] = (double)srcdata[j] / (double)total;
    }

    for (int i = 0; i < height; i++) {
        double*srcdata = mean.ptr<double>(i);
        for (int j = 0; j < width; j++) {
            // 能量
            energy += srcdata[j] * srcdata[j];
            // 熵(entropyropy)
            if (srcdata[j]>0)
                entropy -= srcdata[j] * log(srcdata[j]);
            // 对比度
            contrast += (double)(i - j)*(double)(i - j)*srcdata[j];
            // 逆方差
            idMoment += srcdata[j] / (1 + (double)(i - j)*(double)(i - j));
        }
    }
}
