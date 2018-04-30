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

void MainWindow::on_openImageButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), ".", tr("Image Files(*.png *jpg *bmp)"));
    image = cv::imread(fileName.toStdString());
    QImage img = cvMat2QImage(image);
    ui->displayLabel->setPixmap(QPixmap::fromImage(img));

}

void MainWindow::on_processButton_clicked()
{
    if(!image.data)
        return;
    cv::Mat result;
    cv::Vec3b target(130, 190, 230);
    int mindistance = 100;
    result.create(image.rows, image.cols, CV_8U);
    cv::Mat_<cv::Vec3b>::const_iterator it = image.begin<cv::Vec3b>();
    cv::Mat_<cv::Vec3b>::const_iterator itend = image.end<cv::Vec3b>();
    cv::Mat_<uchar>::iterator itout = result.begin<uchar>();

    for(; it!=itend; ++it, ++itout){
        if(getD4Distance(*it, target) < mindistance)
            *itout = 255;
        else *itout = 0;
    }
    QImage img = cvMat2QImage(result);
    ui->displayLabel->setPixmap(QPixmap::fromImage(img));
}

int MainWindow::getD4Distance(const cv::Vec3b &color, const cv::Vec3b& target) const
{
    return abs(color[0]- target[0]) + abs(color[1]-target[1]) + abs(color[2]- target[2]);
}
