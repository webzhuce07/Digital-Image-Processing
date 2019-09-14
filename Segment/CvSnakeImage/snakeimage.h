/**********************************************************************

Copyright (c) Mr.Bin. All rights reserved.
For more information visit: http://blog.csdn.net/webzhuce

**********************************************************************/
//modify based on cvSnakeImage. The general process of the algorithm is as follows.
//while (!converged) {
//		for (i = 0; i < number_of_points; i++) {
//  			Calculate Econt
//				Calculate Ecurv
//				Calculate Eimg
//				ENERGY = alpha * Econt + beta * Ecurv + gamma * Eimg;
//		}
//}

#pragma once
#include <opencv2/opencv.hpp>

#define SNAKE_BIG 2.e+38f
#define WTILE_SIZE 8  
#define TILE_SIZE (WTILE_SIZE + 2)   

enum  Scheme
{
	SNAKE_IMAGE,  ///image intensity is energy
	SNAKE_GRAD, //magnitude of gradient is energy
};
class SnakeImage
{
public:
	SnakeImage(const cv::Mat &src);
	void segment(std::vector<cv::Point> &points);
	void segment(std::vector<cv::Point> &points, std::vector<float> &alpha, std::vector<float> &beta, std::vector<float> &gamma);
	void setAlpha(float i) { alpha_ = i; }
	float getAlpha() const { return alpha_; }
	void setBeta(float i) { beta_ = i; }
	float getBeta() const { return beta_; }
	void setGamma(float i) { gamma_ = i; }
	float getGamma() const { return gamma_; }

private:
	void drawCurve(std::vector<cv::Point> &points);
	cv::Size win_{ 5, 5 };
	float alpha_ = 0.4;
	float beta_ = 0.6;
	float gamma_ = 0.8;
	Scheme scheme_ = SNAKE_IMAGE;
	cv::Mat src_;
	cv::Mat image_; //for debugging
	cv::TermCriteria termcriteria_;
};
