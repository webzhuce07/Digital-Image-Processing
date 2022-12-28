#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include "box_filter.h"
#include "side_window_box_filter.h"
#include "median_filter.h"
#include "side_window_median_filter.h"

using namespace std;
using namespace cv;

int main(int argc, char *argv[]) {

	Mat inputImage = imread("./render171.png", 1);
	int radius = 4;
	int iteration = 10;
	int height = inputImage.rows;
	int width = inputImage.cols;
	int channel = inputImage.channels();

	BoxFilter box_filter;
	box_filter.init(height, width, radius);
	SideWindowBoxFilter side_window_box_filter;
	side_window_box_filter.init(height, width, radius);

	if (channel == 1)
	{
		Mat inputImageF, outputImageF;
		inputImage.convertTo(inputImageF, CV_32F);
		inputImageF.copyTo(outputImageF);

		box_filter.fastBoxFilter((float*)inputImageF.data, radius, height, width, (float*)outputImageF.data);
		cv::imwrite("result0.jpg", outputImageF);

		inputImageF.copyTo(outputImageF);
		for (int i = 0; i < iteration; ++i) {
			box_filter.fastBoxFilter((float*)outputImageF.data, radius, height, width, (float*)outputImageF.data);
		}
		cv::imwrite("result1.jpg", outputImageF);
		
		side_window_box_filter.fastSideWindowBoxFilter((float*)inputImageF.data, radius, height, width, (float*)outputImageF.data);
		cv::imwrite("result2.jpg", outputImageF);

		inputImageF.copyTo(outputImageF);
		for (int i = 0; i < iteration; ++i) {
			side_window_box_filter.fastSideWindowBoxFilter((float*)outputImageF.data, radius, height, width, (float*)outputImageF.data);
		}
		cv::imwrite("result3.jpg", outputImageF);	
	}
	else
	{
		Mat inputImageF, outputImageF;
		inputImage.convertTo(inputImageF, CV_32F);
		inputImageF.copyTo(outputImageF);

		Mat inputImageFs[3], outputImageFs[3];
		cv::split(inputImageF, inputImageFs);
		cv::split(inputImageF, outputImageFs);
		for (int i = 0; i < channel; ++i) {
			box_filter.fastBoxFilter((float*)inputImageFs[i].data, radius, height, width, (float*)outputImageFs[i].data);
		}
		cv::merge(outputImageFs, 3, outputImageF);
		cv::imwrite("result0.jpg", outputImageF);

		cv::split(inputImageF, outputImageFs);
		for (int i = 0; i < iteration; ++i) {
			for (int c = 0; c < channel; ++c) {
				box_filter.fastBoxFilter((float*)outputImageFs[c].data, radius, height, width, (float*)outputImageFs[c].data);
			}
		}
		cv::merge(outputImageFs, 3, outputImageF);
		cv::imwrite("result1.jpg", outputImageF);

		cv::split(inputImageF, outputImageFs);
		for (int i = 0; i < channel; ++i) {
			side_window_box_filter.fastSideWindowBoxFilter((float*)outputImageFs[i].data, radius, height, width, (float*)outputImageFs[i].data);
		}
		cv::merge(outputImageFs, 3, outputImageF);
		cv::imwrite("result2.jpg", outputImageF);

		cv::split(inputImageF, outputImageFs);
		for (int i = 0; i < iteration; ++i) {
			for (int c = 0; c < channel; ++c) {
				side_window_box_filter.fastSideWindowBoxFilter((float*)outputImageFs[c].data, radius, height, width, (float*)outputImageFs[c].data);
			}
		}
		cv::merge(outputImageFs, 3, outputImageF);
		cv::imwrite("result3.jpg", outputImageF);
	}

	return EXIT_SUCCESS;
	
}


