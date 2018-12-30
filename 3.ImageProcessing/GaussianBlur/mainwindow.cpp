#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "utility.h"
#include <opencv2/core/softfloat.hpp>
#include <QFileDialog>
using namespace cv;

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
    image_ = imread(fileName.toStdString());
    QImage img = cvMat2QImage(image_);
    ui->DisplayLabel->setPixmap(QPixmap::fromImage(img));
    ui->DisplayLabel->resize(ui->DisplayLabel->pixmap()->size());
}

void MainWindow::on_ProcessButton1_clicked()
{
    if(!image_.data)
        return;
    gaussianBlur(image_);
}

void MainWindow::gaussianBlur(const cv::Mat &image)
{
    Mat input;
    image.convertTo(input, CV_32F);
    std::vector<cv::Mat> channels;
    split(image, channels);  //分离图像的RGB通道, OpenCV:BGR
    for(auto& splitedimage : channels){
        //第一步
        int w = getOptimalDFTSize(splitedimage.cols);
        int h = getOptimalDFTSize(splitedimage.rows);
        Mat padded;
        copyMakeBorder(splitedimage, padded, 0, h - splitedimage.rows, 0, w - splitedimage.cols, BORDER_CONSTANT, Scalar::all(0));
        //imshow("padded", padded);
        Mat plane[] = {cv::Mat_<float>(padded), Mat::zeros(padded.size(),CV_32F)};
        Mat compleximage;
        merge(plane, 2, compleximage);
        dft(compleximage, compleximage);
        //第二步
        split(compleximage, plane);//以下的操作是移动图像  (零频移到中心)
        int cx = plane[0].cols / 2;
        int cy = plane[0].rows / 2;
        Mat part1r(plane[0],Rect(0,0,cx,cy));  //元素坐标表示为(cx,cy)
        Mat part2r(plane[0],Rect(cx,0,cx,cy));
        Mat part3r(plane[0],Rect(0,cy,cx,cy));
        Mat part4r(plane[0],Rect(cx,cy,cx,cy));
        Mat temp;
        part1r.copyTo(temp);  //左上与右下交换位置(实部)
        part4r.copyTo(part1r);
        temp.copyTo(part4r);
        part2r.copyTo(temp);  //右上与左下交换位置(实部)
        part3r.copyTo(part2r);
        temp.copyTo(part3r);
        Mat part1i(plane[1],Rect(0,0,cx,cy));  //元素坐标(cx,cy)
        Mat part2i(plane[1],Rect(cx,0,cx,cy));
        Mat part3i(plane[1],Rect(0,cy,cx,cy));
        Mat part4i(plane[1],Rect(cx,cy,cx,cy));
        part1i.copyTo(temp);  //左上与右下交换位置(虚部)
        part4i.copyTo(part1i);
        temp.copyTo(part4i);
        part2i.copyTo(temp);  //右上与左下交换位置(虚部)
        part3i.copyTo(part2i);
        temp.copyTo(part3i);
        //第三步
        Mat gaussianblur(padded.size(), CV_32FC1);
        float sigma = 45.0;
        float d0 = 2* sigma* sigma;
        for(int i = 0;i < padded.rows; i++ ) {
            for(int j = 0; j < padded.cols; j++ ) {
              float d = pow(float(i - padded.rows / 2), 2) + pow(float(j - padded.cols / 2 ), 2);//分子,计算pow必须为float型
              gaussianblur.at<float>(i,j) = expf(-d / d0);//expf为以e为底求幂（必须为float型）
            }
        }
        imshow("GausianBlur", gaussianblur);
        multiply(plane[0], gaussianblur, plane[0]); //滤波（实部与滤波器模板对应元素相乘）
        multiply(plane[1], gaussianblur, plane[1]);//滤波（虚部与滤波器模板对应元素相乘）
        //第四步
        //以下的操作是移动图像  (零频移到左上角)
        Mat rpart1r(plane[0],Rect(0,0,cx,cy));  //元素坐标表示为(cx,cy)
        Mat rpart2r(plane[0],Rect(cx,0,cx,cy));
        Mat rpart3r(plane[0],Rect(0,cy,cx,cy));
        Mat rpart4r(plane[0],Rect(cx,cy,cx,cy));
        rpart1r.copyTo(temp);  //左上与右下交换位置(实部)
        rpart4r.copyTo(rpart1r);
        temp.copyTo(rpart4r);
        rpart2r.copyTo(temp);  //右上与左下交换位置(实部)
        rpart3r.copyTo(rpart2r);
        temp.copyTo(rpart3r);
        Mat rparti(plane[1],Rect(0,0,cx,cy));  //元素坐标(cx,cy)
        Mat rpart2i(plane[1],Rect(cx,0,cx,cy));
        Mat rpart3i(plane[1],Rect(0,cy,cx,cy));
        Mat rpart4i(plane[1],Rect(cx,cy,cx,cy));
        rparti.copyTo(temp);  //左上与右下交换位置(虚部)
        rpart4i.copyTo(rparti);
        temp.copyTo(rpart4i);
        rpart2i.copyTo(temp);  //右上与左下交换位置(虚部)
        rpart3i.copyTo(rpart2i);
        temp.copyTo(rpart3i);
        Mat blur_result;
        merge(plane, 2, blur_result);//实部与虚部合并
        //第五步
        idft(blur_result, blur_result);	//idft结果也为复数
        //第六步
        split(blur_result, plane);//分离通道，主要获取通道
        magnitude(plane[0],plane[1], plane[0]);  //求幅值(模)
        normalize(plane[0],plane[0], 1,0, CV_MINMAX);  //归一化便于显示
        plane[0].copyTo(splitedimage);
    }
    Mat reuslut;
    merge(channels, reuslut);
    imshow("Result", reuslut);
}



