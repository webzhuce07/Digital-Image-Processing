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
    void on_ProcessButton2_clicked();
    void on_ProcessButton3_clicked();
    void on_ProcessButton4_clicked();
    void on_ProcessButton5_clicked();

private:
    void updateDisplayLabel(const cv::Mat& image);
    Ui::MainWindow *ui;
    cv::Mat image_;
    cv::Mat image_gray_;
    int thresh_;
};

#endif // MAINWINDOW_H
