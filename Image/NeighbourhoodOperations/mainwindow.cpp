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

void MainWindow::on_ProcessButton_clicked()
{
    if(!image.data)
        return;
    cv::Mat result;
    sharpen(image, result);
    QImage img = cvMat2QImage(result);
    ui->DisplayLabel->setPixmap(QPixmap::fromImage(img));
    ui->DisplayLabel->resize(ui->DisplayLabel->pixmap()->size());
}

void MainWindow::sharpen(const cv::Mat &image, cv::Mat &result)
{
    result.create(image.size(), image.type());
    const int channels = image.channels() ;
    for(int j = 1; j < image.rows -1; j++){
        const uchar* previous = image.ptr<const uchar>(j-1); //up row
        const uchar* current = image.ptr<const uchar>(j);  // current row
        const uchar* next = image.ptr<const uchar>(j+1); // next row
        uchar* output = result.ptr<uchar>(j); //output row
        for(int i = channels; i < (image.cols -1) * channels; i++)
            *output++ = cv::saturate_cast<uchar>(5*current[i] - current[i-channels] - current[i + channels] - previous[i]- next[i]);
    }
    // Set the unprocess pixels to 0
    cv::Scalar color;
    if(image.channels() == 3)
        color = cv::Scalar(0, 0, 0);
    else  color = cv::Scalar(0);
    result.row(0).setTo(color);
    result.row(result.rows - 1).setTo(color);
    result.col(0).setTo(color);
    result.col(result.cols - 1).setTo(color);
}

void MainWindow::sharpen2D(const cv::Mat &image, cv::Mat &result)
{
    cv::Mat kernel(3, 3, CV_32F, cv::Scalar(0));
    kernel.at<float>(1, 1) = 5.0;
    kernel.at<float>(0, 1) = -1.0;
    kernel.at<float>(2, 1) = -1.0;
    kernel.at<float>(1, 0) = -1.0;
    kernel.at<float>(1, 2) = -1.0;
    cv::filter2D(image, result, image.depth(), kernel);
}

