#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<opencv2/opencv.hpp>
#include <QFileInfo>
using namespace cv;
using namespace cv::ml;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    modelPath_ = "model.dat";
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_Test_clicked()
{
    if(!QFileInfo(modelPath_).exists())
        return;
    cv::Ptr<cv::ml::SVM> svm  = cv::ml::SVM::load(modelPath_.toStdString());
    // visual representation
    int width = 512;
    int height = 521;
    Mat image = Mat::zeros(height, width, CV_8UC3);
    //prediction
    cv::Vec3b green(0, 255, 0);
    cv::Vec3b blue(255, 0, 0);
    for(int i = 0; i< image.rows; i++)
    {
        for(int j = 0; j < image.cols; j++)
        {
            cv::Mat sampleMat = (cv::Mat_<float>(1, 2)<<j, i);
            float respose = svm->predict(sampleMat);
            if(respose == 1)
                image.at<cv::Vec3b>(i,j) = green;
            else if(respose == -1)
                image.at<cv::Vec3b>(i,j) = blue;

        }
    }

    int thickness = -1;
    int lineType = cv::LineTypes::LINE_8;
    cv::circle(image, cv::Point(501, 10), 5, cv::Scalar(0, 0, 0), thickness, lineType);
    cv::circle(image, cv::Point(255, 10), 5, cv::Scalar(255, 255, 255), thickness, lineType);
    cv::circle(image, cv::Point(501, 255), 5, cv::Scalar(255, 255, 255), thickness, lineType);
    cv::circle(image, cv::Point(10, 501), 5, cv::Scalar(255, 255, 255), thickness, lineType);

    thickness = 2;
    lineType = cv::LineTypes::LINE_8;

    cv::Mat sv = svm->getSupportVectors();
    for (int i = 0; i < sv.rows; i++)
    {
        const float* v = sv.ptr<float>(i);
        cv::circle(image, cv::Point((int)v[0], (int)v[1]), 6, cv::Scalar(128, 128, 128), thickness, lineType);
    }
    cv::imshow("SVM Simple Example", image);
}

void MainWindow::on_Train_clicked()
{
   // training data
   int labels[4]= {1, -1, -1, -1};
   float trainingData[4][2] = {{501, 10}, {255, 10}, {501, 255}, {10, 501}};
   Mat trainingDataMat(4, 2, CV_32FC1, trainingData);
   Mat labelsMat(4, 1, CV_32SC1, labels);

   // initial SVM
   cv::Ptr<cv::ml::SVM> svm = cv::ml::SVM::create();
   svm->setType(cv::ml::SVM::Types::C_SVC);
   svm->setKernel(cv::ml::SVM::KernelTypes::LINEAR);
   svm->setTermCriteria(cv::TermCriteria(cv::TermCriteria::MAX_ITER, 100, 1e-6));

   // train operation
   svm->train(trainingDataMat, cv::ml::SampleTypes::ROW_SAMPLE, labelsMat);

   //Save
   svm->save(modelPath_.toStdString());
}
