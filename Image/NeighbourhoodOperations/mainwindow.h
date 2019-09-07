#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <opencv2/opencv.hpp>

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
    void on_ProcessButton_clicked();

private:
    void sharpen(const cv::Mat& image, cv::Mat& result);
     void sharpen2D(const cv::Mat& image, cv::Mat& result);
    Ui::MainWindow *ui;
    cv::Mat image;
};

#endif // MAINWINDOW_H
