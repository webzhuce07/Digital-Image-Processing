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
    void on_OpenVideoButton_clicked();

private:
    cv::Point getMousePosition(QMouseEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void track(const cv::Mat& frame, const cv::Mat& template_mat);
    void displayFrame();
    void displayFrameWithRect();
    Ui::MainWindow *ui;
    cv::Mat frame_ ;
    cv::VideoCapture capture_;
    cv::Rect track_box_;
    bool track_flag_ = false;
    bool draw_flag_ = false;
};

#endif // MAINWINDOW_H
