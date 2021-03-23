#include <fstream>
#include <QMainWindow>
#include "QLineEdit"
#include "ui_mainwindow.h"

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    clock_t start_time;
    clock_t end_time;
    cv::dnn::Net net;
    cv::Mat image;
    QString image_path;

private slots:
    void browse();
    void recognition();

};
