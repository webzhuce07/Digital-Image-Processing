#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "utility.h"
#include <QFileDialog>
#include <QMouseEvent>
#include <vector>

using namespace cv;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    seed_point_(-1, -1)
{
    ui->setupUi(this);
	setWindowTitle("阿Bin先生");
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
    seed_point_ =  cv::Point2i(-1, -1);
}

void MainWindow::mousePressEvent(QMouseEvent* event)
{
    if(!image_.data)
        return;
    QPoint global_point = event->globalPos();
    QPoint point = ui->DisplayImageLabel->mapFromGlobal(global_point);
    //contentsRect表示的是QLabel的内容范围，而pixmap->rect()则表示的图像的实际大小，
    //得出图像相对于QLabel的具体偏移量，然后可以真正将鼠标坐标转换成图像上的点位置
    int xoffset = (ui->DisplayImageLabel->contentsRect().width()-ui->DisplayImageLabel->pixmap()->rect().width())/2;
    int realx = point.x() - xoffset;
    if(( realx >= 0 ) && (realx <= ui->DisplayImageLabel->pixmap()->rect().width())){
        int yoffset = (ui->DisplayImageLabel->contentsRect().height()-ui->DisplayImageLabel->pixmap()->height())/2;
        int realy = point.y() - yoffset;
        seed_point_ = cv::Point2i(realx, realy);
        cv::Mat result = image_.clone();
        cv::circle(result, seed_point_, 1, cv::Scalar(0, 0, 255), 1);
        QImage img = cvMat2QImage(result);
        ui->DisplayImageLabel->setPixmap(QPixmap::fromImage(img));
    }
}

void MainWindow::on_RegionGrowButton_clicked()
{
    if(!image_.data)
        return;
    int throld = 4;
    cv::Mat result = regionGrow(image_, seed_point_, throld);
    namedWindow("regionGrow");
    imshow("regionGrow", result);
    cv::Mat result_fast = regionGrowFast(image_, seed_point_, throld);
    namedWindow("regionGrowFast");
    imshow("regionGrowFast", result_fast);

}

//参考《数字图像处理与机器学习》
cv::Mat MainWindow::regionGrow(const cv::Mat &src, const cv::Point2i seed, int throld)
{
    //convert src to gray for getting gray value of every pixel
    cv::Mat gray;
    cv::cvtColor(src,gray, cv::COLOR_RGB2GRAY);

    // set every pixel to black
    cv::Mat result = cv::Mat::zeros(src.size(), CV_8UC1);
    if((seed.x < 0) || (seed.y < 0))
        return result;
    result.at<uchar>(seed.y, seed.x) = 255;

    //count: sum of points that meet the conditions in every eight-neighbor
    int count = 1;
    //start growing
    while (count > 0) {
        count = 0;
        for(int row = 1; row < gray.rows -1; row++){
            for(int col = 1; col < gray.cols - 1; col++){
                if(result.at<uchar>(row, col) != 255)
                    continue;
                //gray value of current seed
                int seed_gray = gray.at<uchar>(row, col);
                for(int m = row -1; m <= row+1; m++){
                    for(int n = col -1; n <= col+1; n++){
                        int value = gray.at<uchar>(m, n);
                        if((result.at<uchar>(m, n) == 0) && (abs(value - seed_gray) <= throld)){
                            result.at<uchar>(m, n) = 255;
                            count++;
                        }
                    }
                }
            }
        }
    }
    return result;
}

cv::Mat MainWindow::regionGrowFast(const cv::Mat &src, const cv::Point2i seed, int throld)
{
    //convert src to gray for getting gray value of every pixel
    cv::Mat gray;
    cv::cvtColor(src,gray, cv::COLOR_RGB2GRAY);

    // set every pixel to black
    cv::Mat result = cv::Mat::zeros(src.size(), CV_8UC1);
    if((seed.x < 0) || (seed.y < 0))
        return result;
    result.at<uchar>(seed.y, seed.x) = 255;

    //grow direction sequenc
    int grow_direction[8][2] = {{-1,-1}, {0,-1}, {1,-1}, {1,0}, {1,1}, {0,1}, {-1,1}, {-1,0}};
    //seeds collection
    std::vector<cv::Point2i> seeds;
    seeds.push_back(seed);

    //start growing
    while(! seeds.empty()){
        //get a seed
        cv::Point2i current_seed = seeds.back();
        seeds.pop_back();
        //gray value of current seed
        int seed_gray = gray.at<uchar>(current_seed.y, current_seed.x);

        for(int i = 0; i < 8; ++i){
            cv::Point2i neighbor_seed(current_seed.x + grow_direction[i][0], current_seed.y + grow_direction[i][1]);
            //check wether in image
            if(neighbor_seed.x < 0 || neighbor_seed.y < 0 || neighbor_seed.x > (gray.cols-1) || (neighbor_seed.y > gray.rows -1))
                continue;
            int value = gray.at<uchar>(neighbor_seed.y, neighbor_seed.x);
            if((result.at<uchar>(neighbor_seed.y, neighbor_seed.x) == 0) && (abs(value - seed_gray) <= throld)){
                result.at<uchar>(neighbor_seed.y, neighbor_seed.x) = 255;
                seeds.push_back(neighbor_seed);
            }
        }
    }
    return result;
}
