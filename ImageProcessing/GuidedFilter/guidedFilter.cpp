#include "guidedFilter.h"
#include <iostream>
#include <string>

using namespace std;
using namespace cv;


GuidedFilter::GuidedFilter()
{
}

GuidedFilter::~GuidedFilter()
{
}


//引导图为灰度图像
cv::Mat GuidedFilter::runGuidedFilter_Gray(cv::Mat I, cv::Mat P, int type, int radius, double eps)
{
	cv::Size winSize(2 * radius + 1, 2 * radius + 1);
	//求I*I, I*P
	cv::Mat  I2, IP;
	multiply(I, I, I2);
	multiply(I, P, IP);
	//求均值
	cv::Mat meanI, meanP, meanI2, meanIP;
	cv::boxFilter(I, meanI, type, winSize);
	cv::boxFilter(P, meanP, type, winSize);
	cv::boxFilter(I2, meanI2, type, winSize);
	cv::boxFilter(IP, meanIP, type, winSize);
	//求方差/协方差
	cv::Mat varI, covIP;
	varI = meanI2 - meanI.mul(meanI);
	covIP = meanIP - meanI.mul(meanP);
	//求系数a, b
	cv::Mat a, b;
	varI += eps;
	cv::divide(covIP, varI, a);
	b = meanP - a.mul(meanI);
	//a、b窗口内求平均
	cv::Mat meanA, meanB;
	cv::boxFilter(a, meanA, type, winSize);
	cv::boxFilter(b, meanB, type, winSize);
	//输出
	cv::Mat output;
	output = meanA.mul(I) + meanB;
	return output;
}

//引导图I为灰度图像，输入图像P为单通道或者三通道图像
cv::Mat GuidedFilter::myGuidedFilter_GrayGuided(cv::Mat guidedImg, cv::Mat inputImg, int radius, double eps)
{
	CV_Assert(guidedImg.channels() == 1);
	CV_Assert(inputImg.channels() == 1 || inputImg.channels() == 3);
	CV_Assert(guidedImg.rows == inputImg.rows && guidedImg.cols == inputImg.cols);
	int type = CV_64FC1;
	cv::Mat I, P, output;
	inputImg.convertTo(P, type);
	guidedImg.convertTo(I, type);

	//判断输入图像是单通道还是三通道
	int channel = inputImg.channels();
	switch (channel)
	{
	case 1:
		output = runGuidedFilter_Gray(I, P, type, radius, eps);
		break;
	case 3:
	{
		cv::Mat bgr[3], bgrFilter[3];
		cv::split(P, bgr);
		for (int chan = 0; chan < channel; chan++)
		{
			bgrFilter[chan] = runGuidedFilter_Gray(I, bgr[chan], type, radius, eps);
		}
		cv::merge(bgrFilter, channel, output);
		break;
	}
	default:
		cout << "err! input image channel should be 1 or 3! " << endl;
		break;
	}

	return output;
}

//引导图I为三通道图像
cv::Mat GuidedFilter::runGuidedFilter_Color(cv::Mat I, cv::Mat P, int type, int radius, double eps)
{
	cv::Size winSize(2 * radius + 1, 2 * radius + 1);
	int channel = I.channels();
	int H = I.rows;
	int W = I.cols;

	cv::Mat bgr[3], meanI[3];
	//引导图各通道的均值
	split(I, bgr);
	for (int chan = 0; chan < channel; chan++)
	{
		boxFilter(bgr[chan], meanI[chan], type, winSize);
	}
	//输入图像均值
	cv::Mat meanP;
	boxFilter(P, meanP, type, winSize);
	//引导图各通道与输入图像相乘并求均值
	cv::Mat meanIP[3], IP;
	for (int chan = 0; chan < channel; chan++)
	{
		multiply(bgr[chan], P, IP);
		boxFilter(IP, meanIP[chan], type, winSize);
	}
	//引导图各通道与输入图协方差
	cv::Mat covIP[3], meanImulP;
	for (int chan = 0; chan < channel; chan++)
	{
		multiply(meanI[chan], meanP, meanImulP);
		covIP[chan] = meanIP[chan] - meanImulP;
	}

	//求引导图协方差矩阵
	cv::Mat varI[9], tmp, mean2Tmp, meanTmp2;
	int varIdx = 0;
	for (int i = 0; i < channel; i++)
	{
		for (int j = 0; j < channel; j++)
		{
			multiply(bgr[i], bgr[j], tmp);
			boxFilter(tmp, meanTmp2, type, winSize);//mean(I*I)
			multiply(meanI[i], meanI[j], mean2Tmp);//meanI*meanI
			varI[varIdx] = meanTmp2 - mean2Tmp;
			varIdx++;
		}
	}
	//求a，三通道
	cv::Mat a[3];
	for (int chan = 0; chan < channel; chan++)
	{
		a[chan] = cv::Mat::zeros(I.size(), type);
	}
	cv::Mat epsEye = cv::Mat::eye(3, 3, type);
	epsEye *= eps;
	//公式(19)
	for (int y = 0; y < H; y++)
	{
		double* vData[9];
		for (int v = 0; v < 9; v++)
		{
			vData[v] = (double*)varI[v].ptr<double>(y);
		}
		double* cData[3];
		for (int c = 0; c < 3; c++)
		{
			cData[c] = (double *)covIP[c].ptr<double>(y);
		}
		double* aData[3];
		for (int c = 0; c < 3; c++)
		{
			aData[c] = (double*)a[c].ptr<double>(y);
		}
		for (int x = 0; x < W; x++)
		{
			cv::Mat sigma = (cv::Mat_<double>(3, 3) <<
				vData[0][x], vData[1][x], vData[2][x],
				vData[3][x], vData[4][x], vData[5][x],
				vData[6][x], vData[7][x], vData[8][x]
				);
			sigma += epsEye;
			cv::Mat cov_Ip_13 = (cv::Mat_<double>(3, 1) <<
				cData[0][x], cData[1][x], cData[2][x]);
			cv::Mat tmpA = sigma.inv()*cov_Ip_13;
			double* tmpData = tmpA.ptr<double>(0);
			for (int c = 0; c < 3; c++)
			{
				aData[c][x] = tmpData[c];
			}
		}
	}

	//求b
	cv::Mat b = meanP - a[0].mul(meanI[0]) - a[1].mul(meanI[1]) - a[2].mul(meanI[2]);
	//b的均值
	cv::Mat meanB;
	boxFilter(b, meanB, type, winSize);
	//a的均值
	cv::Mat meanA[3];
	for (int c = 0; c < channel; c++)
	{
		boxFilter(a[c], meanA[c], type, winSize);
	}
	cv::Mat output = (meanA[0].mul(bgr[0]) + meanA[1].mul(bgr[1]) + meanA[2].mul(bgr[2])) + meanB;

	return output;
}

//引导图I为三通道图像，输入图像P为单通道或者三通道图像
cv::Mat GuidedFilter::myGuidedFilter_ColorGuided(cv::Mat guidedImg, cv::Mat inputImg, int radius, double eps)
{
	CV_Assert(guidedImg.channels() == 3);
	CV_Assert(inputImg.channels() == 1 || inputImg.channels() == 3);
	CV_Assert(guidedImg.cols == inputImg.cols && guidedImg.rows == inputImg.rows);
	int type = CV_64F;
	int channel = inputImg.channels();
	cv::Mat I, P, output;
	guidedImg.convertTo(I, type);
	inputImg.convertTo(P, type);

	//判断输入图像是单通道还是三通道
	switch (channel)
	{
	case 1:
		output = runGuidedFilter_Color(I, P, type, radius, eps);
		break;
	case 3:
	{
		cv::Mat bgr[3], bgrFilter[3];
		cv::split(P, bgr);
		for (int chan = 0; chan < channel; chan++)
		{
			bgrFilter[chan] = runGuidedFilter_Color(I, bgr[chan], type, radius, eps);
		}
		cv::merge(bgrFilter, channel, output);
		break;
	}
	default:
		cout << "err! input image channel should be 1 or 3! " << endl;
		break;
	}

	output.convertTo(output, CV_8U);
	return output;
}
主函数
#include <opencv2\opencv.hpp>
#include "guidedFilter.h"
#include <iostream>

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
    //if (argc != 2)
    //{
    //    cout << "Usage: opencv_test <image path>" << endl;
    //    return -1;
    //}

    //char *imgName = argv[1]; 
	char *imgName = "C:\\Users\\VINNO\\Desktop\\src\\cat.jpg";
    Mat inputImg;

    inputImg = imread(imgName, 1);
    if (!inputImg.data)
    {
        cout << "No image data" << endl;
        return -1;
    }
    Mat grayImg , guidedImg;
	inputImg.copyTo(guidedImg);

	GuidedFilter filter;
	grayImg = filter.myGuidedFilter_ColorGuided(inputImg, guidedImg, 80, 0.001);
    imwrite("./result.jpg", grayImg);
	imshow("", grayImg);
	waitKey(0);

    return 0;
}