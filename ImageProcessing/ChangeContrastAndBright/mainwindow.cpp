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
    ui->BrightSlider->setValue(0);
    ui->ContrastSlider->setValue(100);
}

void MainWindow::changeContrastAndBright()
{
    if(!image.data)
        return;
    double contrast_value = ui->ContrastSlider->value();
    double bright_value = ui->BrightSlider->value();
    cv::Mat result = image.clone();
    const int channels = result.channels() ;
    for(int j = 0; j < result.rows; j++){
        uchar* current = result.ptr<uchar>(j);  // current row
        for(int i = 0; i < image.cols * channels; i++)
            current[i] = cv::saturate_cast<uchar>(current[i]*(contrast_value *0.01)+ bright_value);
    }
    QImage img = cvMat2QImage(result);
    ui->DisplayLabel->setPixmap(QPixmap::fromImage(img));
    ui->DisplayLabel->resize(ui->DisplayLabel->pixmap()->size());
}

void MainWindow::on_ContrastSlider_valueChanged(int value)
{
    changeContrastAndBright();
}

void MainWindow::on_BrightSlider_valueChanged(int value)
{
    changeContrastAndBright();
}
