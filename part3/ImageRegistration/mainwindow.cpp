#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "utility.h"
#include <QFileDialog>
#include <QMouseEvent>
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
    ui->DisplayImageLabel->setPixmap(QPixmap::fromImage(img));
    ui->DisplayImageLabel->resize(ui->DisplayImageLabel->pixmap()->size());
    reLayoutLabel();
    points_.clear();
}

void MainWindow::on_OpenBaseImageButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), ".", tr("Image Files(*.png *jpg *bmp)"));
    base_image_ = cv::imread(fileName.toStdString());
    QImage img = cvMat2QImage(base_image_);
    ui->DisplayBaseImageLabel->setPixmap(QPixmap::fromImage(img));
    ui->DisplayBaseImageLabel->resize(ui->DisplayBaseImageLabel->pixmap()->size());
    reLayoutLabel();
    base_points_.clear();
}

void MainWindow::on_RegisterButton_clicked()
{
    if((!base_image_.data) || (!image_.data))
        return;
    if(base_points_.empty()|| points_.empty())
        return;
    cv::Mat homo = cv::findHomography(points_, base_points_,CV_RANSAC);
    cv::Mat result = image_.clone();
    cv::warpPerspective(image_, result, homo, cv::Size(image_.cols, image_.rows));
    QImage img = cvMat2QImage(result);
    ui->DisplayImageLabel->setPixmap(QPixmap::fromImage(img));
}

void MainWindow::reLayoutLabel()
{
    QPoint point = ui->DisplayBaseImageLabel->pos();
    point.rx() += (ui->DisplayBaseImageLabel->width()+ 20);
    ui->DisplayImageLabel->move(point);
}

void MainWindow::mousePressEvent(QMouseEvent* event)
{
    QPoint global_point = event->globalPos();
    QPoint base_point = ui->DisplayBaseImageLabel->mapFromGlobal(global_point);
    //contentsRect表示的是QLabel的内容范围，而pixmap->rect()则表示的图像的实际大小，
    //得出图像相对于QLabel的具体偏移量，然后可以真正将鼠标坐标转换成图像上的点位置
    int xoffset = (ui->DisplayBaseImageLabel->contentsRect().width()-ui->DisplayBaseImageLabel->pixmap()->rect().width())/2;
    int realx = base_point.x() - xoffset;
    if(( realx>= 0 ) && (realx <= ui->DisplayBaseImageLabel->pixmap()->rect().width())){
        int yoffset = (ui->DisplayBaseImageLabel->contentsRect().height()-ui->DisplayBaseImageLabel->pixmap()->height())/2;
        int realy = base_point.y() - yoffset;
        cv::Point2f real_point(realx, realy);
        cv::circle(base_image_, real_point, 1, cv::Scalar(0, 0, 255), -1);
        base_points_.push_back(real_point);
        QImage img = cvMat2QImage(base_image_);
        ui->DisplayBaseImageLabel->setPixmap(QPixmap::fromImage(img));
        return;
    }
    QPoint point = ui->DisplayImageLabel->mapFromGlobal(global_point);
    //contentsRect表示的是QLabel的内容范围，而pixmap->rect()则表示的图像的实际大小，
    //得出图像相对于QLabel的具体偏移量，然后可以真正将鼠标坐标转换成图像上的点位置
    xoffset = (ui->DisplayImageLabel->contentsRect().width()-ui->DisplayImageLabel->pixmap()->rect().width())/2;
    realx = point.x() - xoffset;
    if(( realx >= 0 ) && (realx <= ui->DisplayImageLabel->pixmap()->rect().width())){
        int yoffset = (ui->DisplayImageLabel->contentsRect().height()-ui->DisplayImageLabel->pixmap()->height())/2;
        int realy = point.y() - yoffset;
        cv::Point2f real_point(realx, realy);
        cv::circle(image_, real_point, 1, cv::Scalar(0, 0, 255), -1);
        points_.push_back(real_point);
        QImage img = cvMat2QImage(image_);
        ui->DisplayImageLabel->setPixmap(QPixmap::fromImage(img));
        return;
    }
}
