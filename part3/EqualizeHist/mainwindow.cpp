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

void MainWindow::on_EqualizeButton_clicked()
{
    if(!image.data)
        return;
    cv::Mat result = commonEqualize(image);
    QImage img = cvMat2QImage(result);
    ui->DisplayLabel->setPixmap(QPixmap::fromImage(img));
    ui->DisplayLabel->resize(ui->DisplayLabel->pixmap()->size());
}

cv::Mat MainWindow::commonEqualize(const cv::Mat& image)
{
    cv::Mat result = image.clone();
    if(image.channels() == 0){
        cv::equalizeHist(image,result);
        return result;
    }
    const int channels = image.channels();
    cv::Mat* imageRGB = new cv::Mat[channels];
    split(image, imageRGB);
    for(int i=0; i< channels;++i)
        cv::equalizeHist(imageRGB[i],imageRGB[i]);
    cv::merge(imageRGB, channels, result);
    delete[] imageRGB;
    return result;
}
