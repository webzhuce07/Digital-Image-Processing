#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<opencv2/opencv.hpp>
using namespace cv;
using namespace cv::ml;

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

void MainWindow::on_Test_clicked()
{
    //the feature vector of a test sample
    float testfeature[3]= { 6, 130, 8 };
    Mat testdata(1, 3, CV_32FC1, testfeature);
    Ptr<NormalBayesClassifier> model = NormalBayesClassifier::load("normalBayes.txt");
    int result  = model->predict(testdata);
    ui->label->setText(QString("Predict result: %1").arg(result));
}

void MainWindow::on_Train_clicked()
{
    //8 training smaple sets with a smaple feature vector dimension of 3
    float features[8][3] =
    {
        { 6, 180, 12 },
        { 5.92, 190, 11 },
        { 5.58, 170, 12 },
        { 5.92, 165, 10 },
        { 5, 100, 6 },
        { 5.5, 150, 8 },
        { 5.42, 130, 7 },
        { 5.75, 150, 9 },
    };

    //construct the feature vectors of the training smaples
    Mat featuresmat(8, 3, CV_32FC1, features);
    int labels[8] = { 1, 1, 1, 1, -1, -1, -1, -1 };
    //construct the category labels of the training samples
    Mat labelsmat(8, 1, CV_32SC1, labels);
    //train bayesian classifier
    Ptr<NormalBayesClassifier> model = NormalBayesClassifier::create();
    Ptr<TrainData> trainingdata = TrainData::create(featuresmat, ROW_SAMPLE, labelsmat);
    bool trainresult = model->train(trainingdata);
    if (trainresult){
       ui->label->setText("Train successfully!");
       model->save("normalBayes.txt");
   } else ui->label->setText("Train failurely!");
}
