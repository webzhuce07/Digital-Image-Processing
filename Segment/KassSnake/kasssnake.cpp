/**********************************************************************

Copyright (c) Mr.Bin. All rights reserved.
For more information visit: http://blog.csdn.net/webzhuce

**********************************************************************/
#include "kasssnake.h"
using namespace cv;


KassSnake::KassSnake(const cv::Mat& src)
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
}

void KassSnake::segment(std::vector<cv::Point2f>& points)
{
	drawCurve(points);
	//create pentagonal banded matrix
	float a = 2.0 * alpha_ + 6.0 * beta_;
	float b = -1.0 * (alpha_ + 4.0 * beta_);
	float c = beta_;
	const int h = points.size() + 1;
	Mat A = diagonalCycleMat(h, a, b, c);
	A = A + gamma_ * Mat::eye(A.size(), A.type());
	Mat Ainv;
	cv::invert(A, Ainv);

	//E ext = E image + E con
	Mat eline = getEline();
	Mat eedge = getEedge();
	Mat eterm = getEterm();
	Mat eext = wline_ * eline + wedge_ * eedge + wterm_ * eterm;
	//calculate fx, fy
	Mat fx = eext.clone(), fy = eext.clone();
	gradient(eext, fx, fy);
	
	//iteration
	Mat ssx(h, 1, CV_32FC1);
	Mat ssy(h, 1, CV_32FC1);
	Mat sx(h, 1, CV_32FC1);
	Mat sy(h, 1, CV_32FC1);
	for (int t = 0; t < interation_; t++)
	{
		for (int i = 0; i < h; i++)
		{
			cv::Point2f &point = points[i % points.size()];
			ssx.at<float>(i, 0) = gamma_ * point.x - kappa_ * fx.at<float>(point);
			ssy.at<float>(i, 0) = gamma_ * point.y - kappa_ * fy.at<float>(point);
		}
		
		sx = Ainv * ssx;
		sy = Ainv * ssy;
		//update points
		for (int i = 0; i < points.size(); i++)
		{
			float x = sx.at<float>(i, 0);
			x = std::max(0.0f, x);
			x = std::min(x, float(src_.cols - 1));
			points[i].x = x;
			float y = sy.at<float>(i, 0);
			y = std::max(0.0f, y);
			y = std::min(y, float(src_.rows - 1));
			points[i].y = y;
		}
		//for showing
		drawCurve(points);
	}
}

cv::Mat KassSnake::getEline()
{
	return src_.clone();
}

cv::Mat KassSnake::getEedge()
{
	Mat gx, gy, e = src_.clone();
	gradient(src_, gx, gy);
	for (int i = 0; i < src_.rows; i++)
	{
		for (int j = 0; j < src_.cols; j++)
		{
			float x = gx.at<float>(i, j);
			float y = gy.at<float>(i, j);
			e.at<float>(i, j) = -1.0 * std::sqrt(std::pow(x, 2) + std::pow(y, 2));
		}
	}
	return e;
}

cv::Mat KassSnake::getEterm()
{
	Mat src;
	GaussianBlur(src_, src, Size(3, 3), sigma_);
	Mat kernel;
	kernel = (Mat_<float>(1, 2) << -1, 1);
	Mat cx = conv2(src, kernel);
	kernel = (Mat_<float>(2, 1) << -1, 1);
	Mat cy = conv2(src, kernel);
	kernel = (Mat_<float>(1, 3) << 1, -2, 1);
	Mat cxx = conv2(src, kernel);
	kernel = (Mat_<float>(3, 1) << 1, -2, 1);
	Mat cyy = conv2(src, kernel);
	kernel = (Mat_<float>(2, 2) << 1, -1, -1, 1);
	Mat cxy = conv2(src, kernel);
	
	Mat e = src_.clone();
	for (int i = 0; i < src_.rows; i++)
	{
		for (int j = 0; j < src_.cols; j++)
		{
			float x = cx.at<float>(i, j);
			float y = cy.at<float>(i, j);
			float xx = cxx.at<float>(i, j);
			float yy = cyy.at<float>(i, j);
			float xy = cxy.at<float>(i, j);
			float sxy = x * x + y * y + 1.0f;
			e.at<float>(i, j) = (yy * x * x - 2.0 * xy * x * y + xx * y * y) / std::pow(sxy, 1.5);
		}
	}
	return e;
}

cv::Mat KassSnake::diagonalCycleMat(int n, float a, float b, float c)
{
	const int len = 5;
	const int h = len / 2;
	float J[5] = { c, b, a, b, c };
	Mat A = Mat::zeros(n, n, CV_32FC1);
	for (int i = 0; i < n; i++)
	{
		int j = i;
		A.at<float>(i, j) = J[h];
		int k = 1;
		while (k <= h)
		{
			if (j - k < 0)
				j = j + n;
			A.at<float>(i, j - k) = J[h - k];
			if (j + k > (n-1))
				j = j - n;
			A.at<float>(i, j + k) = J[h + k];
			k++;
		}
	}
	return A;
}

//same as C = conv2(A,B,'same')
cv::Mat KassSnake::conv2(const cv::Mat &src, cv::Mat &kernel)
{
	//rotate kernel 180 degrees for convolution
	flip(kernel, kernel, 0);
	flip(kernel, kernel, 1);
	Mat dstImage = src.clone();
	int x = kernel.cols - 1;
	int y = kernel.rows - 1;
	copyMakeBorder(src, dstImage, y, y, x, x, BORDER_CONSTANT, 0);
	x = kernel.cols / 2.0;
	y = kernel.rows / 2.0;
	filter2D(dstImage, dstImage, dstImage.depth(), kernel, cv::Point(0, 0));
	return dstImage(cv::Rect(x, y, src.cols, src.rows));
}

void KassSnake::gradient(const cv::Mat &src, cv::Mat &gradx, cv::Mat &grady)
{
	if (src.rows < 2 || src.cols < 2)
		return;
	src.copyTo(gradx);
	src.copyTo(grady);
	for (int i = 0; i < src.rows; i++)
	{
		for (int j = 0; j < src.cols; j++)
		{
			if(j == 0)
				gradx.at<float>(i, j) = src.at<float>(i, j + 1) - src.at<float>(i, j);
			else if(j == src.cols -1)
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

void KassSnake::drawCurve(std::vector<cv::Point2f> &points)
{
	cv::Mat image = image_.clone();
	for (int i = 0; i < points.size(); i++)
	{
		int j = (i + 1) % points.size();
		cv::line(image, points[i], points[j], CV_RGB(255, 0, 0), 3, 8, 0);
	}
	cv::imshow("Iteration", image);
	cv::waitKey(0);
}

void KassSnake::print(cv::Mat &src)
{
	for (int i = 0; i < src.rows; i++)
	{
		for (int j = 0; j<src.cols; j++)
		{
			std::cout << src.at<float>(i, j) << "  ";
		}
		std::cout << std::endl;
	}
}

