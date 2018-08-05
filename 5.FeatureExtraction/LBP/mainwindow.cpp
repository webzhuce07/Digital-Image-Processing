#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "utility.h"
#include "math.h"
#include <QFileDialog>

using namespace cv;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
	setWindowTitle("阿Bin先生");
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
    baseLBP(gray, result);
    QImage img = cvMat2QImage(result);
    ui->DisplayLabel->setPixmap(QPixmap::fromImage(img));
    ui->DisplayLabel->resize(ui->DisplayLabel->pixmap()->size());
}

void MainWindow::baseLBP(const cv::Mat src, cv::Mat &dst)
{
    dst.create(src.size(), src.type());
    for(int i = 1; i < src.rows- 1; i++){
        for(int j = 1; j < src.cols- 1; j++) {
            uchar code = 0;
            uchar center = src.at<uchar>(i,j);
            code |= (src.at<uchar>(i-1,j-1) >= center) << 7;
            code |= (src.at<uchar>(i-1,j  ) >= center) << 6;
            code |= (src.at<uchar>(i-1,j+1) >= center) << 5;
            code |= (src.at<uchar>(i  ,j+1) >= center) << 4;
            code |= (src.at<uchar>(i+1,j+1) >= center) << 3;
            code |= (src.at<uchar>(i+1,j  ) >= center) << 2;
            code |= (src.at<uchar>(i+1,j-1) >= center) << 1;
            code |= (src.at<uchar>(i  ,j-1) >= center) << 0;
            dst.at<uchar>(i,j) = code;
        }
    }

}
