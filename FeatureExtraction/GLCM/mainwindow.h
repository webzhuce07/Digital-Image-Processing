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
    void on_ProcessButton1_clicked();

private:
    void getGLCM0(cv::Mat& src, cv::Mat& dst, int gray_level);
    void getGLCM90(cv::Mat& src, cv::Mat& dst, int gray_level);
    void getGLCM45(cv::Mat& src, cv::Mat& dst, int gray_level);
    void getGLCM135(cv::Mat& src, cv::Mat& dst, int gray_level);
    void featureGLCM(cv::Mat&src, double& Asm, double& Ent, double& Con, double& Idm);
    Ui::MainWindow *ui;
    cv::Mat image_;
};

#endif // MAINWINDOW_H
