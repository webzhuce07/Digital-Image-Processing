/**********************************************************************
Copyright (c) Mr.Bin. All rights reserved.
For more information visit: http://blog.csdn.net/webzhuce
**********************************************************************/
/**
* @brief   This is a filter that implements  KASS,M. Snake : Active
*          Contour Models[J].1988, 4.
*/
#ifndef KassSnake_h
#define KassSnake_h
#include <opencv2/opencv.hpp>

class KassSnake
{
public:
	KassSnake(const cv::Mat& src);
	void segment(std::vector<cv::Point2f>& points);

private:
	cv::Mat getEline();
	cv::Mat getEedge();
	cv::Mat getEterm();
	cv::Mat diagonalCycleMat(int n, float a, float b, float c);
	cv::Mat conv2(const cv::Mat &src, cv::Mat &dst);
	void gradient(const cv::Mat &src, cv::Mat &gradx, cv::Mat &grady);
	void drawCurve(std::vector<cv::Point2f> &points);
	void print(cv::Mat &src);
	float alpha_ = 0.4;  // continuity
	float beta_ = 0.2; // smooth
	float kappa_ = 0.05;
	float gamma_ = 1.0;
	float wline_ = 0.0;
	float wedge_ = 0.4;
	float wterm_ = 0.0;	
	float sigma_ = 1.0; //for Gaussian filter
	cv::Mat src_;
	cv::Mat image_; //for showing
	int interation_ = 1000;
};

#endif
