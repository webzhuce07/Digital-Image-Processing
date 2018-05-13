#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "utility.h"
#include <opencv2/core/softfloat.hpp>
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

void MainWindow::on_ProcessButton3_clicked()
{
    if(!image.data)
        return;
    cv::Mat result = image.clone();
    QImage img = cvMat2QImage(result);
    ui->DisplayLabel->setPixmap(QPixmap::fromImage(img));
    ui->DisplayLabel->resize(ui->DisplayLabel->pixmap()->size());
}

void MainWindow::on_ProcessButton2_clicked()
{
    if(!image.data)
        return;
    cv::Mat result;
    bilinearInterpolation(image, result, 0.5, 0.5);
    QImage img = cvMat2QImage(result);
    ui->DisplayLabel->setPixmap(QPixmap::fromImage(img));
    ui->DisplayLabel->resize(ui->DisplayLabel->pixmap()->size());
}

void MainWindow::on_ProcessButton1_clicked()
{
    if(!image.data)
        return;
    cv::Mat result;
    cv::resize(image, result,cv::Size(), 0.5, 0.5);
    QImage img = cvMat2QImage(result);
    ui->DisplayLabel->setPixmap(QPixmap::fromImage(img));
    ui->DisplayLabel->resize(ui->DisplayLabel->pixmap()->size());
}

void MainWindow::bilinearInterpolation(const cv::Mat& input, cv::Mat& output, double fx, double fy)
{
    //recreate output
    int width = input.cols * fx;
    int height = input.rows*fy;
    output.create(height, width, input.type());
    for(int row =0; row< output.rows;++row){
        float temp_row = (float)((row + 0.5)*(1/fy)- 0.5);
        int in_row = cvFloor(temp_row);
        temp_row -= in_row;
        in_row = std::min(in_row, input.rows - 2);
        in_row = std::max(0, in_row);
        short cbufy[2];
        cbufy[0]= cv::saturate_cast<short>((1.0f- temp_row)*2048);
        cbufy[1]= 2048 - cbufy[0];
        for(int column = 0; column < output.cols; ++column){
            float temp_column = (float)((column + 0.5)*(1/fx) - 0.5);
            int in_column = cvFloor (temp_column);
            temp_column -= in_column;
            if(in_column< 0){
                temp_column = 0;
                in_column = 0;
            }
            if(in_column >= (input.cols - 1)){
                 temp_column = 0;
                 in_column = input.cols -2;
            }
            short cbufx[2];
            cbufx[0]= cv::saturate_cast<short>((1.0f-temp_column)* 2048);
            cbufx[1] = 2048 -cbufx[0];

            uchar* out_data = output.ptr<uchar>(row);  // current row of output
            out_data += (column*output.channels());
            const uchar* in_data00 = input.ptr<const uchar>(in_row);
            in_data00 += (in_column*input.channels());
            const uchar* in_data01 = input.ptr<const uchar>(in_row + 1);
            in_data01 += (in_column*input.channels());
            const uchar* in_data10 = input.ptr<const uchar>(in_row);
            in_data10 += ((in_column+1)*input.channels());
            const uchar* in_data11 = input.ptr<const uchar>(in_row + 1);
            in_data11 += ((in_column+1)*input.channels());
            for(int k = 0; k < output.channels(); ++k)
                *(out_data + k) = (*(in_data00+ k)*cbufx[0]*cbufy[0]
                        + *(in_data01 + k)*cbufx[0]*cbufy[1]
                        +  *(in_data10 + k)*cbufx[1]*cbufy[0]
                        + *(in_data11 + k)*cbufx[1]*cbufy[1]) >> 22;
        }
    }
}
