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
    image_ = cv::imread(fileName.toStdString());
    QImage img = cvMat2QImage(image_);
    ui->DisplayLabel->setPixmap(QPixmap::fromImage(img));
    ui->DisplayLabel->resize(ui->DisplayLabel->pixmap()->size());
}

void MainWindow::on_ProcessButton1_clicked()
{
    if(!image_.data)
        return;
    dftTransform(image_);
}

void MainWindow::dftTransform(cv::Mat &image)
{
    image.convertTo(image, CV_32F);
    std::vector<cv::Mat> channels;
    split(image, channels);  //分离图像的RGB通道，
    cv::Mat image_B = channels[0]; //OpenCV:BGR
    //expand input image to optimal size
    int m1 = cv::getOptimalDFTSize(image_B.rows);  //选取最适合做fft的宽和高
    int n1 = cv::getOptimalDFTSize(image_B.cols);
    cv::Mat padded;
    //填充0
    cv::copyMakeBorder(image_B, padded, 0, m1 - image_B.rows, 0, n1 - image_B.cols, cv::BORDER_CONSTANT, cv::Scalar::all(0));
    cv::Mat planes[] = { cv::Mat_<float>(padded), cv::Mat::zeros(padded.size(), CV_32F) };
    cv::Mat complexI;
    cv::merge(planes, 2, complexI);  //planes[0], planes[1]是实部和虚部

    cv::dft(complexI, complexI, cv::DFT_SCALE | cv::DFT_COMPLEX_OUTPUT);
    cv::split(complexI, planes);

    //定义幅度谱和相位谱
    cv::Mat ph, mag, idft;
    cv::phase(planes[0], planes[1], ph);
    cv::magnitude(planes[0], planes[1], mag);  //由实部planes[0]和虚部planes[1]得到幅度谱mag和相位谱ph

    cv::imshow("phase", ph);
    cv::imshow("magnitude", mag);
    /*
    如果需要对实部planes[0]和虚部planes[1]，或者幅度谱mag和相位谱ph进行操作，在这里进行更改
    */


    cv::polarToCart(mag, ph, planes[0], planes[1]);  //由幅度谱mag和相位谱ph恢复实部planes[0]和虚部planes[1]
    cv::merge(planes, 2, idft);
    cv::dft(idft, idft, cv::DFT_INVERSE | cv::DFT_REAL_OUTPUT);
    image_B = idft(cv::Rect(0, 0, image.cols & -2, image.rows & -2));
    image_B.copyTo(channels[0]);
    merge(channels, image);
    image.convertTo(image, CV_8U);
    cv::imshow("idft", image);
}



