/**********************************************************************

Copyright (c) Mr.Bin. All rights reserved.
For more information visit: http://blog.csdn.net/webzhuce

**********************************************************************/
#pragma once
#include <opencv2/opencv.hpp>
#include "gcgraph.hpp"

class GraphCut
{
public:
	GraphCut(const cv::Mat &image);
	void setLambda(float i) { lambda_ = i; }
	float getLambda() const { return lambda_; }
	void setSigma(float i) { sigma_ = i; }
	float getSigma() const { return sigma_; }
	cv::Mat runInitially(const std::vector<cv::Point> &objectseeds, const std::vector<cv::Point> &backgroundseeds);

private:
	enum GraphCutClasses
	{
		NO_INDICATING = 0, // not belong to Object or Background
		OBJECT = 1,
		BACKGROUND = 2,
	};
	float lambda_ = 1.0;
	float sigma_ = 2.0;
	cv::Mat image_;
	cv::Mat mask_;	
	GCGraph<double> graph_;
};
