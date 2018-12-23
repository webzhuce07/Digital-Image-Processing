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
    image_ = cv::imread(fileName.toStdString());
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

void MainWindow::gaussianBlur(cv::Mat &image)
{
    image.convertTo(image, CV_32F);
    std::vector<cv::Mat> channels;
    split(image, channels);  //分离图像的RGB通道，
    cv::Mat image_B = channels[0]; //OpenCV:BGR
    //第一步
    int w = getOptimalDFTSize(image_B.cols);
    int h = getOptimalDFTSize(image_B.rows);
    Mat padded;
    copyMakeBorder(image_B, padded, 0, h - image_B.rows, 0, w - image_B.cols, BORDER_CONSTANT, Scalar::all(0));
    padded.convertTo(padded, CV_32F);
    imshow("padded", padded);
    Mat plane[]={cv::Mat_<float>(padded), Mat::zeros(padded.size(),CV_32F)};
    Mat complexImg;
    merge(plane, 2, complexImg);
    dft(complexImg,complexImg);
    //第二步
    split(complexImg, plane);
    int cx=plane[0].cols/2;int cy=plane[0].rows/2;//以下的操作是移动图像  (零频移到中心)
    Mat part1_r(plane[0],Rect(0,0,cx,cy));  //元素坐标表示为(cx,cy)
    Mat part2_r(plane[0],Rect(cx,0,cx,cy));
    Mat part3_r(plane[0],Rect(0,cy,cx,cy));
    Mat part4_r(plane[0],Rect(cx,cy,cx,cy));
    Mat temp;
    part1_r.copyTo(temp);  //左上与右下交换位置(实部)
    part4_r.copyTo(part1_r);
    temp.copyTo(part4_r);
    part2_r.copyTo(temp);  //右上与左下交换位置(实部)
    part3_r.copyTo(part2_r);
    temp.copyTo(part3_r);
    Mat part1_i(plane[1],Rect(0,0,cx,cy));  //元素坐标(cx,cy)
    Mat part2_i(plane[1],Rect(cx,0,cx,cy));
    Mat part3_i(plane[1],Rect(0,cy,cx,cy));
    Mat part4_i(plane[1],Rect(cx,cy,cx,cy));
    part1_i.copyTo(temp);  //左上与右下交换位置(虚部)
    part4_i.copyTo(part1_i);
    temp.copyTo(part4_i);
    part2_i.copyTo(temp);  //右上与左下交换位置(虚部)
    part3_i.copyTo(part2_i);
    temp.copyTo(part3_i);
    //第三步
    Mat gaussian_blur(padded.size(),CV_32FC1);
    float sigma = 10.0;
    float d0=2* sigma* sigma;
    for(int i=0;i < padded.rows ; i++ ) {
        for(int j=0; j < padded.cols ; j++ ) {
          float d=pow(float(i - padded.rows/2),2)+pow(float(j - padded.cols/2),2);//分子,计算pow必须为float型
          gaussian_blur.at<float>(i,j)=expf(-d/d0);//expf为以e为底求幂（必须为float型）
        }
    }
    imshow("高斯低通滤波器",gaussian_blur);
    Mat blur_r,blur_i,blur_result;
    multiply(plane[0], gaussian_blur, blur_r); //滤波（实部与滤波器模板对应元素相乘）
    multiply(plane[1], gaussian_blur, blur_i);//滤波（虚部与滤波器模板对应元素相乘）
    Mat plane1[]={blur_r, blur_i};
    merge(plane1,2,blur_result);//实部与虚部合并
    //第四步
    //第五步
    idft(blur_result, blur_result);	//idft结果也为复数
    //第六步
    split(blur_result, plane);//分离通道，主要获取通道
    magnitude(plane[0],plane[1],plane[0]);  //求幅值(模)
    normalize(plane[0],plane[0],1,0,CV_MINMAX);  //归一化便于显示
    imshow("Result", plane[0]);
}



