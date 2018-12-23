#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <opencv2/opencv.hpp>
#include <vector>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_OpenImageButton_clicked();
    void on_RegionGrowButton_clicked();

private:
    cv::Mat regionGrow(const cv::Mat& src, const cv::Point2i seed, int throld);
     cv::Mat regionGrowFast(const cv::Mat& src, const cv::Point2i seed, int throld);
    void mousePressEvent(QMouseEvent* event);
    Ui::MainWindow *ui;
    cv::Mat image_;
    cv::Point2i seed_point_;
};

#endif // MAINWINDOW_H
