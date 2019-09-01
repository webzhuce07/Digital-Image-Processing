/**********************************************************************

Copyright (c) Mr.Bin. All rights reserved.
For more information visit: http://blog.csdn.net/webzhuce

**********************************************************************/
#include<opencv2/opencv.hpp>
using namespace cv;

int main(int argc, char *argv[])
{
    Mat originImage1(Size(180, 120), CV_8UC1, Scalar(0));
    int i, j;
    for(i = 10; i < 80; ++i){
        for(j = 15; j < 75; ++j){
            originImage1.at<uchar>(i,j) = 255;
        }
    }
    for(i = 55; i < 105; ++i){
        for(j = 85; j < 135; ++j){
            originImage1.at<uchar>(i,j) = 255;
        }
    }
    for(i = 25; i < 55; ++i){
        for(j = 140; j < 170; ++j){
            originImage1.at<uchar>(i,j) = 255;
        }
    }
    imshow("OriginImage1", originImage1);


    Mat s1(Size(58, 58), CV_8UC1, Scalar(0));
    for(i = 4; i < 54; ++i){
        for(j = 4; j < 54; ++j){
            s1.at<uchar>(i,j) = 255;
        }
    }
    imshow("S1", s1);

    Mat output1;
    erode(originImage1, output1, s1);
    imshow("Output1", output1);

    Mat originImage2;
    bitwise_not(originImage1,originImage2);
    imshow("OriginImage2", originImage2);

    Mat s2;
    bitwise_not(s1, s2);
    imshow("S2", s2);

    Mat output2;
    erode(originImage2, output2, s2);
    imshow("Output2", output2);

    Mat output;
    bitwise_and(output1, output2, output);
    imshow("Output", output);

    waitKey(0);
    return 0;

}
