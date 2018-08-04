#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "utility.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    thresh_(167)
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
    cv::cvtColor(image_, image_gray_, CV_BGR2GRAY);
    updateDisplayLabel(image_);
}

void MainWindow::on_ProcessButton1_clicked()
{
    if(!image_gray_.data)
        return;
    cv::Mat result;
    cv::threshold(image_gray_, result, thresh_,255, cv::THRESH_OTSU);
    updateDisplayLabel(result);

}

void MainWindow::on_ProcessButton2_clicked()
{
    if(!image_gray_.data)
        return;
    cv::Mat result;
    cv::threshold(image_gray_, result, thresh_,255, cv::THRESH_BINARY);
    updateDisplayLabel(result);
}

void MainWindow::on_ProcessButton3_clicked()
{
    if(!image_gray_.data)
        return;
    cv::Mat result;
    cv::threshold(image_gray_, result, thresh_,255, cv::THRESH_TRUNC);
    updateDisplayLabel(result);
}

void MainWindow::on_ProcessButton4_clicked()
{
    if(!image_gray_.data)
        return;
    cv::Mat result;
    cv::threshold(image_gray_, result, thresh_,255, cv::THRESH_TOZERO);
    updateDisplayLabel(result);
}

void MainWindow::on_ProcessButton5_clicked()
{
    if(!image_gray_.data)
        return;
    cv::Mat result;
    cv::adaptiveThreshold(image_gray_, result, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, 7, 0);
    updateDisplayLabel(result);
}

void MainWindow::updateDisplayLabel(const cv::Mat& image)
{
    QImage img = cvMat2QImage(image);
    ui->DisplayLabel->setPixmap(QPixmap::fromImage(img));
    ui->DisplayLabel->resize(ui->DisplayLabel->pixmap()->size());
}
