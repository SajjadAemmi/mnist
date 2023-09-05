#include <iostream>
#include "QFile"
#include "QDir"
#include "QtDebug"
#include "QMessageBox"
#include "QFileDialog"
#include "QTextStream"
#include "mainwindow.h"
#include "time.h"
#include <vector>


using namespace std;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->btn_browse, SIGNAL(clicked()), this, SLOT(browse()));
    connect(ui->btn_recognition, SIGNAL(clicked()), this, SLOT(recognition()));

//    connect(sudoku, SIGNAL(signalHideCell(int, int)), this, SLOT(slotHideCell(int, int)));

    net = cv::dnn::readNet("./mnist.onnx");
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::browse()
{
    image_path = QFileDialog::getOpenFileName(this, tr("Open Image File"), "/home", tr("JPEG (*.jpg *.jpeg);;PNG (*.png)"));
    ui->tb_path->setText(image_path);

    std::string image_path_str = image_path.toUtf8().constData();
    image = cv::imread(image_path_str);

    QImage qimage = QImage((uchar*) image.data, image.cols, image.rows, image.step, QImage::Format_RGB888);
    QPixmap pixmap = QPixmap::fromImage(qimage);
    ui->lbl_preview->setPixmap(pixmap);
}


void MainWindow::recognition()
{
    start_time = clock();

    cv::Mat image_small, image_small_gray, image_vector;
    int input_width, input_height;

    cv::resize(image, image_small, cv::Size(28, 28));
    cv::cvtColor(image_small, image_small_gray, cv::COLOR_BGR2GRAY);

    input_width = 28;
    input_height = 28;

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

    ui->lbl_output->setText(QString::number(maxLoc.x));
    qDebug() << "class: " << maxLoc.x;

//     cv::namedWindow("Display Image", cv::WINDOW_AUTOSIZE);
//     cv::imshow("Display Image", image);
//     cv::waitKey(0);

    end_time = clock();
    qDebug() << float(end_time - start_time) / CLOCKS_PER_SEC << " seconds";
}

