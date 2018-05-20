#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "utility.h"
#include "math.h"
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
    cv::Mat result;
    BGR2HSI(image_, result);
    QImage img = cvMat2QImage(result);
    ui->DisplayLabel->setPixmap(QPixmap::fromImage(img));
    ui->DisplayLabel->resize(ui->DisplayLabel->pixmap()->size());
}

void MainWindow::on_ProcessButton2_clicked()
{
    if(!image_.data)
        return;
    cv::Mat result;
    cv::cvtColor(image_, result, cv::COLOR_RGB2BGR);
    QImage img = cvMat2QImage(result);
    ui->DisplayLabel->setPixmap(QPixmap::fromImage(img));
    ui->DisplayLabel->resize(ui->DisplayLabel->pixmap()->size());
}


void MainWindow::on_ProcessButton3_clicked()
{
    if(!image_.data)
        return;
    cv::Mat result;
    cv::cvtColor(image_, result, cv::COLOR_RGB2GRAY);
    QImage img = cvMat2QImage(result);
    ui->DisplayLabel->setPixmap(QPixmap::fromImage(img));
    ui->DisplayLabel->resize(ui->DisplayLabel->pixmap()->size());
}

void MainWindow::BGR2HSI(const cv::Mat src, cv::Mat &dst)
{
    if(src.channels() != 3)
        return;
    dst.create(src.rows, src.cols, src.type());
    double r,g,b,h,s,i;
    for(int row = 0; row < src.rows; ++row){
        for(int col = 0; col < src.cols; ++col){
           r = src.at<cv::Vec3b>(row, col)[0] / 255.0;
           g = src.at<cv::Vec3b>(row, col)[1] / 255.0;
           b = src.at<cv::Vec3b>(row, col)[2] / 255.0;
           i = (r + b + g) / 3;

           double min = std::min(r, std::min(b, g));
           if(i < 0.078431)
               s = 0.0;
           else if(i > 0.920000)
               s = 0.0;
           else
               s = 1.0 - 3.0 * min / ( r + g + b);

          double max = std::max(r, std::max(b, g));
          if(max == min){
              h = 0.0;
              s = 0.0;
          }else {
               h = 0.5 *(r - g + r - b) /sqrt((r - g)*( r - g) + (g -b)*(r-b));
               if( h > 0.9999999999)
                   h = 0.0;
               else if(h < -0.9999999999)
                   h = 180.0;
               else
                   h =  acos(h) * 180.0 /  3.14159265358979323846;

               if(b > g)
                   h = 360.0 - h;
          }
          dst.at<cv::Vec3b>(row, col)[0] = h;
          dst.at<cv::Vec3b>(row, col)[1] = s * 255;
          dst.at<cv::Vec3b>(row, col)[2] = i * 255;
        }
    }
}
