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
    void on_OpenSourceImageButton_clicked();
    void on_OpenTemplateImageButton_clicked();
    void on_comboBox_activated(int index);

private:
    void reLayoutLabel();
    void showMatchingResult();
    cv::Point match();
    Ui::MainWindow *ui;
    cv::Mat source_image_;
    cv::Mat template_image_;
};

#endif // MAINWINDOW_H
