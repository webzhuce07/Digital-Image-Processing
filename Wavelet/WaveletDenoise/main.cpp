/**********************************************************************

Copyright (c) Mr.Bin. All rights reserved.
For more information visit: http://blog.csdn.net/webzhuce

**********************************************************************/
# include<opencv2/opencv.hpp>
# include<iostream>
#include "wavelet.h"

using namespace std;
using namespace cv;
Mat AddGaussianNoise(Mat &srcimag);

int main() {
    Mat image = imread("../WaveletDenoise/image.png", 0);
	imshow("OriginImage", image);

	Mat noiseimage = AddGaussianNoise(image);
	imshow("NoiseImage", noiseimage);

	Mat wavledenoise = WaveletThresholdDenoise(noiseimage, 2, 10);
	imshow("WaveletDenoise", wavledenoise);
	waitKey(0);
	return 0;
}


double GenerateGaussianNoise(double mu, double sigma)
{
	//定义小值
	const double epsilon = numeric_limits<double>::min();
	static double z0, z1;
	static bool flag = false;
	flag = !flag;
	//flag为假构造高斯随机变量X
	if (!flag)
		return z1 * sigma + mu;
	double u1, u2;
	//构造随机变量
	do
	{
		u1 = rand() * (1.0 / RAND_MAX);
		u2 = rand() * (1.0 / RAND_MAX);
	} while (u1 <= epsilon);
	//flag为真构造高斯随机变量
	z0 = sqrt(-2.0*log(u1))*cos(2 * CV_PI*u2);
	z1 = sqrt(-2.0*log(u1))*sin(2 * CV_PI*u2);
	return z0*sigma + mu;
}

//add gaussian noise to image
Mat AddGaussianNoise(Mat &srcimag)
{
	Mat dstimage = srcimag.clone();
	int channels = dstimage.channels();
	int rowsnumber = dstimage.rows;
	int colsnumber = dstimage.cols*channels;
	if (dstimage.isContinuous())
	{
		colsnumber *= rowsnumber;
		rowsnumber = 1;
	}
	for (int i = 0; i < rowsnumber; i++)
	{
		for (int j = 0; j < colsnumber; j++)
		{
			int val = dstimage.ptr<uchar>(i)[j] +
				GenerateGaussianNoise(2, 0.8) * 32;
			if (val < 0)
				val = 0;
			if (val > 255)
				val = 255;
			dstimage.ptr<uchar>(i)[j] = (uchar)val;
		}
	}
	return dstimage;
}
