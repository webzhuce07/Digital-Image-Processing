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
    void on_Test_clicked();
    void on_Train_clicked();

private:
    Ui::MainWindow *ui;
    QString modelPath_;
};

#endif // MAINWINDOW_H
