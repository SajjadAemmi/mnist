#include <iostream>
#include "QFile"
#include "QDir"
#include "QtDebug"
#include "QMessageBox"
#include "QTextStream"
#include "mainwindow.h"
#include "time.h"
#include <vector>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

//using namespace cv;
using namespace std;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

//    connect(ui->btn_solve, SIGNAL(clicked()), this, SLOT(solveGame()));
//    connect(ui->btn_new_game, SIGNAL(clicked()), this, SLOT(newGame()));

//    sudoku = new Sudoku();
//    connect(sudoku, SIGNAL(signalShowCell(int, int, int)), this, SLOT(slotShowCell(int, int, int)));
//    connect(sudoku, SIGNAL(signalHideCell(int, int)), this, SLOT(slotHideCell(int, int)));
//    connect(sudoku, SIGNAL(solved()), this, SLOT(solved()));
//    connect(ui->cb_preview, SIGNAL(toggled(bool)), sudoku, SLOT(setPreview(bool)));

    this->process();
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::process()
{
    cv::Mat image, image_small, image_small_gray, image_vector;
    int input_width, input_height;

    image = cv::imread("/home/sajjad/Documents/mnist/0.png");

//    if ( !image.data )
//    {
//      qDebug() << "No image data \n";
//      return;
//    }

    cv::resize(image, image_small, cv::Size(28, 28));
    cv::cvtColor(image_small, image_small_gray, cv::COLOR_BGR2GRAY);

    input_width = 28;
    input_height = 28;

    cv::dnn::Net net = cv::dnn::readNet("/home/sajjad/Documents/mnist/mnist.onnx");

    cv::Mat blob;
    // Create a 4D blob from a frame.
    image_vector = image_small_gray.reshape (1, image_small_gray.rows * image_small_gray.cols);

    cv::dnn::blobFromImage(image_vector, blob);

    net.setInput(blob, "");
    vector<cv::Mat> outs;
    net.forward(outs);

    double minVal, maxVal;
    cv::Point minLoc, maxLoc;

    minMaxLoc(outs[0], &minVal, &maxVal, &minLoc, &maxLoc);

    qDebug() << "besco" << maxLoc.x;

     cv::namedWindow("Display Image", cv::WINDOW_AUTOSIZE);
     cv::imshow("Display Image", image);
     cv::waitKey(0);

//    end_time = clock();
//    qDebug() << float(end_time - start_time) / CLOCKS_PER_SEC << " seconds";
}

