#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "utility.h"
#include <QFileDialog>
#include <QMouseEvent>

using namespace cv;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("阿Bin先生");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mousePressEvent(QMouseEvent* event)
{
    if(!frame_.data)
        return;
    if(event->button() == Qt::LeftButton){
        draw_flag_ = true;
        Point pos = getMousePosition(event);
        if(pos.x < 0 || pos.y < 0)
            return;
        track_box_.x = pos.x;
        track_box_.y = pos.y;
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if(!frame_.data)
        return;
    if(!draw_flag_)
        return;
    Point pos = getMousePosition(event);
    if(pos.x < 0 || pos.y < 0)
        return;
    track_box_.x = track_box_.x < pos.x ? track_box_.x : pos.x;
    track_box_.y = track_box_.y < pos.y ? track_box_.y : pos.y;
    track_box_.width = abs(track_box_.x - pos.x);
    track_box_.height = abs(track_box_.y - pos.y);
    displayFrameWithRect();
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if(!frame_.data)
        return;
    if(draw_flag_){
        track_flag_ = true;
        draw_flag_ = false;
    }
}

void MainWindow::track(const Mat& frame, const Mat& template_mat)
{
    double t = (double)cvGetTickCount();

    Mat gray;
    cvtColor(frame, gray, CV_RGB2GRAY);
    Rect search_window;
    search_window.width = track_box_.width * 3;
    search_window.height = track_box_.height * 3;
    search_window.x = track_box_.x + track_box_.width * 0.5 - search_window.width * 0.5;
    search_window.y = track_box_.y + track_box_.height * 0.5 - search_window.height * 0.5;
    search_window &= Rect(0, 0, frame.cols, frame.rows);
    Mat similarity;
    matchTemplate(gray(search_window), template_mat, similarity, CV_TM_CCOEFF_NORMED);
    double mag_r;
    Point point;
    minMaxLoc(similarity, 0, &mag_r, 0, &point);
    //update track_box_
    track_box_.x = point.x + search_window.x;
    track_box_.y = point.y + search_window.y;

    t = (double)cvGetTickCount() - t;
    std::cout << "cost time: " << t / (double)cvGetTickFrequency() << std::endl; //ms
}

void MainWindow::displayFrame()
{
    QImage img = cvMat2QImage(frame_);
    ui->DisplayImageLabel->setPixmap(QPixmap::fromImage(img));
    ui->DisplayImageLabel->resize(ui->DisplayImageLabel->pixmap()->size());
}

void MainWindow::displayFrameWithRect()
{
    Mat image = frame_.clone();
    rectangle(image, track_box_, Scalar(0, 0, 255), 3);
    QImage img = cvMat2QImage(image);
    ui->DisplayImageLabel->setPixmap(QPixmap::fromImage(img));
    ui->DisplayImageLabel->resize(ui->DisplayImageLabel->pixmap()->size());
}

cv::Point MainWindow::getMousePosition(QMouseEvent *event)
{
    QPoint global_point = event->globalPos();
    QPoint point = ui->DisplayImageLabel->mapFromGlobal(global_point);
    int xoffset = (ui->DisplayImageLabel->contentsRect().width()-ui->DisplayImageLabel->pixmap()->rect().width())/2;
    int realx = point.x() - xoffset;
    if(( realx >= 0 ) && (realx <= ui->DisplayImageLabel->pixmap()->rect().width())){
        int yoffset = (ui->DisplayImageLabel->contentsRect().height()-ui->DisplayImageLabel->pixmap()->height())/2;
        int realy = point.y() - yoffset;
        return Point(realx, realy);
    }
    return Point(-1, -1);
}

void MainWindow::on_OpenVideoButton_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Open Video"), ".", tr("Video Files(*.mpg *avi *mp4)"));
    if(filename.isEmpty())
        return;
    track_flag_ = false;
    capture_ = VideoCapture(filename.toStdString());
    //Init camera
    if (!capture_.isOpened()) {
        std::cout << "capture device failed to open!" << endl;
        return;
    }
    cvNamedWindow("Tracker", CV_WINDOW_AUTOSIZE);
    while(!track_flag_){
        capture_ >> frame_;
        displayFrame();
        imshow("Tracker", frame_);
        if (cvWaitKey(20) == 'q')
            return ;
    }
    Mat gray;
    cvtColor(frame_, gray, CV_RGB2GRAY);
    Mat template_mat = gray(track_box_);
    while(true) {
        capture_ >> frame_;
        if (frame_.empty())
            return;
       // tracking
       track(frame_, template_mat);
       displayFrameWithRect();
       cvtColor(frame_, gray, CV_RGB2GRAY);
       template_mat = gray(track_box_);
       if(cvWaitKey(1) == 27)
           break;
       }
}
