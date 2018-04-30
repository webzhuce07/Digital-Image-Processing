#include <opencv2/opencv.hpp>
#include <iostream>
using namespace cv;

int main()
{
    Mat img = imread("E:/TestData/lena.jpg");
	if (img.empty())
	{
		std::cout << "Error: load image failed." << std::endl;
		return -1;
	}
	namedWindow("image", WINDOW_AUTOSIZE);
	imshow("image", img);
	waitKey();
	return 0;
}
