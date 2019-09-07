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
    void on_ContrastSlider_valueChanged(int value);
    void on_BrightSlider_valueChanged(int value);

private:
    void changeContrastAndBright();
    Ui::MainWindow *ui;
    cv::Mat image;
};

#endif // MAINWINDOW_H
