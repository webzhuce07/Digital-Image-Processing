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
    void on_OpenBaseImageButton_clicked();
    void on_RegisterButton_clicked();

private:
    void reLayoutLabel();
    void mousePressEvent(QMouseEvent* event);
    Ui::MainWindow *ui;
    cv::Mat image_;
    cv::Mat base_image_;
    std::vector<cv::Point2f> points_;
    std::vector<cv::Point2f> base_points_;
};

#endif // MAINWINDOW_H
