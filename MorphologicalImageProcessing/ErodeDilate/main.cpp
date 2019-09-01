#include <opencv2/opencv.hpp>
#include <iostream>
using namespace cv;

int main()
{
    Mat image = imread("../ErodeDilate/erode_dilate.bmp");
    if (image.empty())
	{
		std::cout << "Error: load image failed." << std::endl;
		return -1;
	}
    imshow("Image", image);

    Mat gray;
    cvtColor(image, gray, CV_BGR2GRAY);
    Mat element = getStructuringElement(MORPH_RECT, Size(5, 5));
    Mat dilate;
    cv::dilate(gray, dilate, element);
    imshow("Dilate", dilate);

    Mat erode;
    cv::erode(gray, erode, element);
    imshow("Erode", erode);

    Mat open;
    morphologyEx(gray, open, MORPH_OPEN, element);
    imshow("Open", open);

    Mat close;
    morphologyEx(gray, close, MORPH_CLOSE,element);
    imshow("Close", close);

	waitKey();
	return 0;
}
