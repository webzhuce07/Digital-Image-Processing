/**********************************************************************

Copyright (c) Mr.Bin. All rights reserved.
For more information visit: http://blog.csdn.net/webzhuce

**********************************************************************/
#include "levelset.h"

using namespace cv;
using namespace std;

LevelSet::LevelSet(const cv::Mat &src)
{
	if (src.channels() == 3)
	{
		cv::cvtColor(src, src_, CV_BGR2GRAY);
		src.copyTo(image_);
	}
	else
	{
		src.copyTo(src_);
		cv::cvtColor(src, image_, CV_GRAY2BGR);
	}
	src_.convertTo(src_, CV_32FC1);
	phi_ = Mat::zeros(src_.size(), CV_32FC1);
	dirac_ = Mat::zeros(src_.size(), CV_32FC1);
	heaviside_ = Mat::zeros(src_.size(), CV_32FC1);
}

void LevelSet::initializePhi(cv::Point2f center, float radius)
{
	const float c = 2.0f;
	float value = 0.0;
	for (int i = 0; i < src_.rows; i++)
	{
		for (int j = 0; j < src_.cols; j++)
		{
			value = -sqrt(pow((j - center.x), 2) + pow((i - center.y), 2)) + radius;
			if (abs(value) < 0.1) //
				phi_.at<float>(i, j) = 0;
			else if (value >=0.1) //inside
				phi_.at<float>(i, j) = c;
			else
				phi_.at<float>(i, j) = -c;
		}
	}
}

void LevelSet::gradient(const cv::Mat &src, cv::Mat &gradx, cv::Mat &grady)
{
	if (src.rows < 2 || src.cols < 2)
		return;
	src.copyTo(gradx);
	src.copyTo(grady);
	for (int i = 0; i < src.rows; i++)
	{
		for (int j = 0; j < src.cols; j++)
		{
			if (j == 0)
				gradx.at<float>(i, j) = src.at<float>(i, j + 1) - src.at<float>(i, j);
			else if (j == src.cols - 1)
				gradx.at<float>(i, j) = src.at<float>(i, j) - src.at<float>(i, j - 1);
			else
				gradx.at<float>(i, j) = (src.at<float>(i, j + 1) - src.at<float>(i, j - 1)) / 2.0;
		}
	}
	for (int j = 0; j < src.cols; j++)
	{
		for (int i = 0; i < src.rows; i++)
		{
			if (i == 0)
				grady.at<float>(i, j) = src.at<float>(i + 1, j) - src.at<float>(i, j);
			else if (i == src.rows - 1)
				grady.at<float>(i, j) = src.at<float>(i, j) - src.at<float>(i - 1, j);
			else
				grady.at<float>(i, j) = (src.at<float>(i + 1, j) - src.at<float>(i - 1, j)) / 2.0;
		}
	}

}

void LevelSet::dirac()
{
	const float k1 = epsilon_ / CV_PI;
	const float k2 = epsilon_*epsilon_;
	for (int i = 0; i < src_.rows; i++)
	{
		for (int j = 0; j < src_.cols; j++)
		{
			dirac_.at<float>(i, j) = k1 / (k2 + pow(phi_.at<float>(i, j),2));
		}
	}
}

void LevelSet::heaviside()
{
	const float k3 = 2 / CV_PI;
	for (int i = 0; i < src_.rows; i++)
	{
		for (int j = 0; j < src_.cols; j++)
		{
			heaviside_.at<float>(i, j) = 0.5 * (1 + k3 * atan(phi_.at<float>(i, j) / epsilon_));
		}
	}
}

void LevelSet::calculateCurvature()
{
	Mat dx, dy;
	gradient(src_, dx, dy);
	Mat norm;
	pow(dx.mul(dx) + dy.mul(dy), 0.5, norm);
	Mat dxx, dxy, dyx, dyy;
	gradient(dx / norm, dxx, dxy);
	gradient(dy / norm, dyx, dyy);
	curv_ = dxx + dyy;
}

void LevelSet::calculatC()
{
	c1_ = 0.0f;
	c2_ = 0.0f;
	float sum1 = 0.0f;
	float h1 = 0.0f;
	float sum2 = 0.0f;
	float h2 = 0.0f;
	int outsidenum = 0;
	float value = 0.0f;
	float h = 0.0f;
	for (int i = 0; i < src_.rows; i++)
	{
		for (int j = 0; j < src_.cols; j++)
		{
			value = src_.at<float>(i, j);
			h = heaviside_.at<float>(i, j);
			h1 += h;
			sum1 = h *  value;
			h2 += (1 - h);
			sum2 += (1 - h) * value;
		}
	}
	c1_ = sum1 / (h1 + 1e-10);
	c2_ = sum2 / (h2 + 1e-10);
}

void LevelSet::showEvolving()
{
	Mat image = image_.clone();
	Mat mask = phi_ >= 0;
	cv::dilate(mask, mask, Mat(), Point(-1, -1), 3);
	cv::erode(mask, mask, Mat(), Point(-1, -1), 3);
	vector<vector<Point> > contours;
	findContours(mask, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);
	drawContours(image, contours, -1, CV_RGB(0, 255,0), 2);
	imshow("Evolving", image);
	waitKey(0);
}

void LevelSet::evolving()
{
	showEvolving();
	//iteration
	for (int i = 0; i < iterationnum_; i++)
	{
		heaviside();
		dirac();
		calculateCurvature();
		calculatC();

		//update phi
		for (int i = 0; i < src_.rows; i++)
		{
			for (int j = 0; j < src_.cols; j++)
			{
				float curv = curv_.at<float>(i, j);
				float dirac = dirac_.at<float>(i, j);
				float u0 = src_.at<float>(i, j);;

				float lengthTerm = mu_* dirac * curv;
				float areamterm = nu_ * dirac;
				float fittingterm = dirac * (-lambda1_ * pow(u0 - c1_, 2) + lambda2_ * pow(u0 - c2_, 2));
				float term = lengthTerm + areamterm + fittingterm;
				phi_.at<float>(i, j) = phi_.at<float>(i, j) + timestep_ * term;
			}
		}
		//just for showing
		showEvolving();
	}
}
