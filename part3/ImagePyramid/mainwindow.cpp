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
    cv::pyrUp(image_,result, cv::Size(image_.cols *2, image_.rows * 2));
    QImage img = cvMat2QImage(result);
    ui->DisplayLabel->setPixmap(QPixmap::fromImage(img));
    ui->DisplayLabel->resize(ui->DisplayLabel->pixmap()->size());
}

void MainWindow::on_ProcessButton2_clicked()
{
    if(!image_.data)
        return;
    cv::Mat result;
    cv::pyrDown(image_,result, cv::Size(image_.cols / 2, image_.rows / 2));
    QImage img = cvMat2QImage(result);
    ui->DisplayLabel->setPixmap(QPixmap::fromImage(img));
    ui->DisplayLabel->resize(ui->DisplayLabel->pixmap()->size());
}


void MainWindow::on_ProcessButton3_clicked()
{
    if(!image_.data)
        return;
    cv::Mat G1;
    cv::pyrDown(image_,G1, cv::Size(image_.cols / 2, image_.rows / 2));
    cv::Mat G0;
    cv::pyrUp(G1, G0, cv::Size(G1.cols * 2, G1.rows * 2));
    cv::Mat L0 = image_ - G0;
    QImage img = cvMat2QImage(L0);
    ui->DisplayLabel->setPixmap(QPixmap::fromImage(img));
    ui->DisplayLabel->resize(ui->DisplayLabel->pixmap()->size());
}
