#include <iostream>
#include <time.h>
#include <vector>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>


using namespace std;

int main() 
{
  clock_t start_time, end_time;
  start_time = clock();
  cv::Mat image, image_small, image_small_gray;
  int input_width, input_height;

  image = cv::imread("3.png");
  if ( !image.data )
  {
    cout << "No image data \n";
    return -1;
  }

  resize(image, image, cv::Size(28, 28));
  cvtColor(image, image, cv::COLOR_BGR2GRAY);

  input_width = 28;
  input_height = 28;

  cv::dnn::Net net = cv::dnn::readNet("mnist.onnx");
  // vector<string> outNames = {"feature_fusion/Conv_7/Sigmoid", "feature_fusion/concat_3"};

  cv::Mat blob;
  // cv::Scalar mean = (123.68, 116.78, 103.94);
  // Create a 4D blob from a frame.
  image = image.reshape (1, image.rows * image.cols);
 
  cv::dnn::blobFromImage(image, blob);

  net.setInput(blob, "");
  vector<cv::Mat> outs;
  net.forward(outs);

  cout << outs[0] << endl;

  // cv::namedWindow("Display Image", cv::WINDOW_AUTOSIZE);
  // cv::imshow("Display Image", image_small_gray);
  // cv::waitKey(0);

  end_time = clock();
  cout << float(end_time - start_time) / CLOCKS_PER_SEC << " seconds" << endl;
  return 0;
}

// cmake -S . -B build -DCMAKE_OSX_ARCHITECTURES=arm64
// cmake --build build
