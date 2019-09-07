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
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), ".", tr("Image Files(*.png *jpg *bmp)"));
    cv::Mat logo = cv::imread(fileName.toStdString());
    if(!logo.data)
        return;
    cv::Mat result = image.clone();
    int y = (image.rows > logo.rows) ? (image.rows - logo.rows) : 0;
    int x = (image.cols > logo.cols) ? (image.cols - logo.cols) : 0;
    int height = (image.rows > logo.rows) ? logo.rows : image.rows;
    int width = (image.cols > logo.cols) ? logo.cols : image.cols;
    cv::Mat imageROI =  result(cv::Rect(x,y ,width, height));
    //insert logo
    cv::addWeighted(imageROI, 1.0, logo, 0.3, 0.0, imageROI);
    QImage img = cvMat2QImage(result);
    ui->DisplayLabel->setPixmap(QPixmap::fromImage(img));
    ui->DisplayLabel->resize(ui->DisplayLabel->pixmap()->size());
}

