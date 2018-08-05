#ifndef __mainwindow_h__
#define __mainwindow_h__

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
    void on_SegmentButton_clicked();

private:
    Ui::MainWindow *ui;
    cv::Mat image_;
};

#endif // __mainwindow_h__
