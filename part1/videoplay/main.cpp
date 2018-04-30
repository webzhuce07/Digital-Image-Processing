#include <opencv2/opencv.hpp>
#include <iostream>
using namespace cv;

int main()
{
    //read video
    VideoCapture capture("E:/TestData/1.avi");

    //display every frame
    while (1) {
        Mat frame;
        capture>>frame; //read current frame
        imshow("Read Video", frame); //display current frame
        waitKey(30); //delay 30ms
    }
	return 0;
}
