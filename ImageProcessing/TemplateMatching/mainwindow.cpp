#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "utility.h"
#include <QFileDialog>
#include <QMouseEvent>
#include <QDebug>

using namespace cv;

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

void MainWindow::reLayoutLabel()
{
    QPoint point = ui->DisplaySourceImageLabel->pos();
    point.rx() += (ui->DisplaySourceImageLabel->width()+ 20);
    ui->DisplayTemplateImageLabel->move(point);
}

void MainWindow::showMatchingResult()
{
    if(template_image_.cols > source_image_.cols || template_image_.rows > source_image_.rows)
        return;
    Point pos = match();
    Mat result_image = source_image_.clone();
    rectangle(result_image, pos, Point( pos.x + template_image_.cols , pos.y + template_image_.rows ), Scalar::all(0), 2, 8, 0 );
    QImage img = cvMat2QImage(result_image);
    ui->DisplaySourceImageLabel->setPixmap(QPixmap::fromImage(img));
    ui->DisplaySourceImageLabel->resize(ui->DisplaySourceImageLabel->pixmap()->size());
    reLayoutLabel();
}

cv::Point MainWindow::match()
{
    //create result output mat
    int result_cols =  source_image_.cols - template_image_.cols + 1;
    int result_rows = source_image_.rows - template_image_.rows + 1;
    Mat result( result_cols, result_rows, CV_32FC1 );
    //match and normalize
    int match_method = ui->comboBox->currentIndex();
    matchTemplate(source_image_, template_image_, result, match_method);
    normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());

    //get the best matching position by using minMaxLoc
    double minVal, maxVal;
    Point minLoc, maxLoc;
    minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );

    //For the first two methods ( TM_SQDIFF and MT_SQDIFF_NORMED ) the best match are the lowest values.
    //For all the others, higher values represent better matches.
    if( match_method  == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED )
        return minLoc;
    return  maxLoc;
}

void MainWindow::on_OpenSourceImageButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), ".", tr("Image Files(*.png *jpg *bmp)"));
    source_image_ = cv::imread(fileName.toStdString());
    if(!template_image_.data){
        QImage img = cvMat2QImage(source_image_);
        ui->DisplaySourceImageLabel->setPixmap(QPixmap::fromImage(img));
        ui->DisplaySourceImageLabel->resize(ui->DisplaySourceImageLabel->pixmap()->size());
        reLayoutLabel();
        return;
    }
    showMatchingResult();
}

void MainWindow::on_OpenTemplateImageButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), ".", tr("Image Files(*.png *jpg *bmp)"));
    template_image_ = cv::imread(fileName.toStdString());
    QImage img = cvMat2QImage(template_image_);
    ui->DisplayTemplateImageLabel->setPixmap(QPixmap::fromImage(img));
    ui->DisplayTemplateImageLabel->resize(ui->DisplayTemplateImageLabel->pixmap()->size());
    reLayoutLabel();
    if(source_image_.data)
        showMatchingResult();
}

void MainWindow::on_comboBox_activated(int index)
{
    if(source_image_.data && template_image_.data)
        showMatchingResult();
}
