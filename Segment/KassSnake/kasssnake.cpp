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

}

void KassSnake::segment(std::vector<cv::Point>& points)
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
	Sobel(eext, fx, eext.depth(), 1, 0);
	Sobel(eext, fy, eext.depth(), 0, 1);
	
	//iteration
	Mat ssx(h, 1, CV_32FC1);
	Mat ssy(h, 1, CV_32FC1);
	Mat sx(h, 1, CV_32FC1);
	Mat sy(h, 1, CV_32FC1);
	for (int i = 0; i < h; i++)
	{
		sx.at<float>(i, 0) = points[i % points.size()].x;
		sy.at<float>(i, 0) = points[i % points.size()].y;
	}
	for (int t = 0; t < 400; t++)
	{
		for (int i = 0; i < h; i++)
		{
			cv::Point &point = points[i % points.size()];
			ssx.at<float>(i, 0) = gamma_ * point.x - 0.1 * fx.at<float>(point);
			ssy.at<float>(i, 0) = gamma_ * point.y - 0.1 * fy.at<float>(point);
		}
		
		sx = Ainv * ssx;
		sy = Ainv * ssy;
		//for showing
		for (int i = 0; i < points.size(); i++)
		{
			int x = sx.at<float>(i, 0);
			x = std::max(0, x);
			x = std::min(x, src_.cols - 1);
			points[i].x = x;
			int y = sy.at<float>(i, 0);
			y = std::max(0, y);
			y= std::min(y, src_.rows - 1);
			points[i].y = y;
		}
		drawCurve(points);
	}
}

cv::Mat KassSnake::getEline()
{
	Mat e;
	src_.convertTo(e, CV_32FC1);
	return e;
}

cv::Mat KassSnake::getEedge()
{
	Mat gx, gy, e;
	src_.convertTo(gx, CV_32FC1);
	src_.convertTo(gy, CV_32FC1);
	src_.convertTo(e, CV_32FC1);
	Sobel(src_, gx, gx.depth(), 1, 0);
	Sobel(src_, gy, gy.depth(), 0, 1);
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
	src_.convertTo(src, CV_32FC1);
	GaussianBlur(src, src, Size(5, 5), sigma_);
	Mat cx;
	Sobel(src, cx, src.depth(), 1, 0);
	Mat cy;
	Sobel(src, cy, src.depth(), 0, 1);
	Mat cxx;
	Sobel(cx, cxx, src.depth(), 1, 0);
	Mat cyy;
	Sobel(cy, cyy, src.depth(), 0, 1);
	Mat cxy;
	Sobel(cx, cxy, src.depth(), 0, 1);	
	Mat e = src.clone();
	for (int i = 0; i < src_.rows; i++)
	{
		for (int j = 0; j < src_.cols; j++)
		{
			float x = cx.at<float>(i, j);
			float y = cy.at<float>(i, j);
			float xx = cxx.at<float>(i, j);
			float yy = cyy.at<float>(i, j);
			float xy = cxy.at<float>(i, j);
			float sxy = x * x + y * y;
			if (sxy > 1e-9)
				e.at<float>(i, j) = (yy * x * x - 2.0 * xy * x * y + xx * y * y) / std::pow(sxy, 1.5);
			else
				e.at<float>(i, j) = 0.0f;
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

void KassSnake::drawCurve(std::vector<cv::Point> &points)
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

