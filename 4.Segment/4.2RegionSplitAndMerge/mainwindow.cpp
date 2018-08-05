#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "utility.h"
#include <QFileDialog>
#include <QMouseEvent>
#include <vector>
#include "splitandmerge.h"

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

void MainWindow::on_OpenImageButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), ".", tr("Image Files(*.png *jpg *bmp)"));
    image_ = cv::imread(fileName.toStdString());
    QImage img = cvMat2QImage(image_);
    ui->DisplayImageLabel->setPixmap(QPixmap::fromImage(img));
    ui->DisplayImageLabel->resize(ui->DisplayImageLabel->pixmap()->size());
}

void MainWindow::on_SegmentButton_clicked()
{
    if(!image_.data)
        return;
    SplitAndMerge a(image_);
    a.excute();
}
