/**********************************************************************

Copyright (c) Mr.Bin. All rights reserved.
For more information visit: http://blog.csdn.net/webzhuce

**********************************************************************/
#include "graphcut.h"

using namespace cv;

GraphCut::GraphCut(const cv::Mat &image)
{
	image.copyTo(image_);
	mask_ = Mat(image_.rows, image_.cols, CV_8UC1, Scalar(NO_INDICATING));
}

cv::Mat GraphCut::runInitially(const std::vector<cv::Point> &objectseeds, const std::vector<cv::Point> &backgroundseeds)
{
	Mat image; // copy of image_ for computing
	if (image_.channels() == 3)
	{
		cvtColor(image_, image, cv::COLOR_RGB2GRAY);
	}
	else
	{
		image_.copyTo(image);
	}
	
	//update mask and compute intensity distributions
	uchar objecthist[256] = { 0 };
	int objectcount = 0;
	uchar backgroundhist[256] = { 0 };
	int backgroundcount = 0;
	uchar pixel = 0;
	for (auto &p : objectseeds)
	{
		pixel = image.at<uchar>(p);
		++objecthist[pixel];
		++objectcount;
		mask_.at<uchar>(p) = OBJECT;
	}
	for (auto &p : backgroundseeds)
	{
		pixel = image.at<uchar>(p);
		++backgroundhist[pixel];
		++backgroundcount;
		mask_.at<uchar>(p) = BACKGROUND;
	}

	//compute weight and create graph
	int vtxCount = image.cols * image.rows,
		edgeCount = 2 * (4 * image.cols * image.rows - 3 * (image.cols + image.rows) + 2);
	graph_.create(vtxCount, edgeCount);
	std::vector<cv::Point> neighbors;
	neighbors.push_back({ -1, 0 }); //left
	neighbors.push_back({ 1, 0 }); //right
	neighbors.push_back({ 0, -1 }); //up
	neighbors.push_back({ 0, 1 }); //down

	neighbors.push_back({ -1, -1 }); //left-up
	neighbors.push_back({ -1, 1 }); //left-down
	neighbors.push_back({ 1, -1 }); //right-up
	neighbors.push_back({ 1, 1 }); //right-down
	Point p, neighborp;

	//compute neighbor weights(B{p,q}) and K
	std::vector<std::vector<double>> neighborweights;
	neighborweights.reserve(vtxCount);
	double maxsum = 0.0, sum = 0.0, weight = 0;
	uchar neighborpixel = 0;
	for (p.y = 0; p.y < image.rows; p.y++)
	{
		for (p.x = 0; p.x < image.cols; p.x++)
		{
			pixel = image.at<uchar>(p);
			std::vector<double> weights;
			sum = 0.0;
			for (auto &offset : neighbors)
			{
				neighborp.x = p.x + offset.x;
				neighborp.y = p.y + offset.y;
				if (neighborp.x < 0 || neighborp.x >= image.cols ||
					neighborp.y < 0 || neighborp.y >= image.rows)
				{
					weights.push_back(0.0);
					continue;
				}
				neighborpixel = image.at<uchar>(neighborp);
				weight = -1.0 * std::pow((pixel - neighborpixel), 2) / (2.0 * std::pow(sigma_, 2));
				weight = exp(weight) / std::sqrt(std::pow(offset.x, 2) + std::pow(offset.y, 2));
				weights.push_back(weight);
				sum += weight;				
			}
			neighborweights.push_back(weights);
			if (maxsum < sum)
				maxsum = sum;
		}
	}
	double k = 1 + maxsum;

	neighbors.clear();
	neighbors.push_back({ -1, 0 }); //left
	//neighbors.push_back({ 1, 0 }); //right
	neighbors.push_back({ 0, -1 }); //up
	//neighbors.push_back({ 0, 1 }); //down

	neighbors.push_back({ -1, -1 }); //left-up
	//neighbors.push_back({ -1, 1 }); //left-down
	neighbors.push_back({ 1, -1 }); //right-up
	//neighbors.push_back({ 1, 1 }); //right-down

	for (p.y= 0;  p.y < image.rows; p.y++)
	{
		for (p.x = 0; p.x < image.cols; p.x++)
		{
			// add node
			int vtxIdx = graph_.addVtx();
			pixel = image.at<uchar>(p);

			// set t-weights
			double fromSource, toSink;
			if (mask_.at<uchar>(p) == NO_INDICATING)
			{
				//Pr(0, 1)
				fromSource = -log(std::max((int)backgroundhist[pixel], 1) / (double)backgroundcount) * lambda_;
				toSink = -log(std::max((int)objecthist[pixel], 1) / (double)objectcount) * lambda_;
				//std::cout << fromSource << " " << toSink << std::endl;
			}
			else if (mask_.at<uchar>(p) == BACKGROUND)
			{
				fromSource = 0;
				toSink = k;
			}
			else // OBJECT
			{
				fromSource = k;
				toSink = 0;
			}
			graph_.addTermWeights(vtxIdx, fromSource, toSink);

			// set n-weights
			for (int i = 0;i < neighbors.size(); i++)
			{
				auto &offset = neighbors[i];
				neighborp.x = p.x + offset.x;
				neighborp.y = p.y + offset.y;
				if (neighborp.x < 0 || neighborp.x >= image.cols ||
					neighborp.y < 0 || neighborp.y >= image.rows)
					continue;
				weight = neighborweights[p.x + p.y * image.cols][i * 2];
				graph_.addEdges(vtxIdx, vtxIdx + offset.x + offset.y * image.cols, weight, weight);
			}	
		}
		
	}

	//result
	Mat result;
	image_.copyTo(result);
	graph_.maxFlow();
	for (p.y = 0; p.y < result.rows; p.y++)
	{
		for (p.x = 0; p.x < result.cols; p.x++)
		{
			if (!graph_.inSourceSegment(p.y * image.cols + p.x /*vertex index*/))
			{
				if(result.channels() == 1)
					result.at<uchar>(p) = 0;
				else
				{
					result.at<cv::Vec3b>(p)[0] = 0;
					result.at<cv::Vec3b>(p)[1] = 0;
					result.at<cv::Vec3b>(p)[2] = 255;
				}
			}			
		}
	}
	return result;
}

