/**********************************************************************

Copyright (c) Mr.Bin. All rights reserved.
For more information visit: http://blog.csdn.net/webzhuce

**********************************************************************/
#include<opencv2\opencv.hpp>
#include<iostream>
using namespace std;
using namespace cv;

Point seed;

//handle image of eight connected boundaries
//boundary is white
Mat RegionFill(Mat &src, Point seed)
{
    Mat dst = Mat::zeros(src.size(), src.type());
    cv::Mat element = cv::getStructuringElement(cv::MORPH_CROSS, cv::Size(3, 3));
    //complement of dst
    Mat invertimage;
    bitwise_not(src, invertimage);
    //add seed to dst
    dst.at<uchar>(seed.y, seed.x) = 255;
    while (true)
    {
        Mat temp;
        dst.copyTo(temp);
        dilate(dst, dst, element);
        //not exceed the boundary
        bitwise_and(dst, invertimage, dst);
        //no change
        if (cv::countNonZero(dst - temp) == 0)
        {
            break;
        }
    }
    return dst;
}
void OnMouseHandle(int event, int x, int y, int flag, void* param)
{
    Mat& src = *(cv::Mat*)param;
    switch (event)
    {
    case EVENT_LBUTTONDOWN:
        seed.x = x;
        seed.y = y;
        break;
    case EVENT_LBUTTONUP:
        Mat image;
        src.copyTo(image);
        if(src.at<uchar>(seed) == 255)
            bitwise_not(image, image);
        Mat result = RegionFill(image, seed);
        if(src.at<uchar>(seed) == 255)
            bitwise_not(result, result);
        imshow("FilledImage", result);
        break;
    }
}

int main()
{
    Mat src = imread("../RegionFill/head_boundary.bmp", 0);
    imshow("OriginImage", src);

    //region fill
    setMouseCallback("OriginImage", OnMouseHandle, (void*)&src);
    waitKey(0);
    return 0;
}
