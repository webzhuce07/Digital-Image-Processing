#pragma once
#include <opencv2/opencv.hpp>

enum WaveletFamilies
{
	Harr,
	Symlets_2
};

enum WaveletThresholdTpye
{
	Hard,
	Soft
};

//Decomposition
void WaveletFunction(cv::Mat &lowfilter, cv::Mat &highfilter, WaveletFamilies name = Harr);
//Reconstruction
void WaveletIFunction(cv::Mat &lowfilter, cv::Mat &highfilter, WaveletFamilies name = Harr);
void DWT(cv::Mat input, cv::Mat output, const cv::Mat &lowfilter, const cv::Mat &highfilter);
void DWT(cv::Mat input, cv::Mat output, WaveletFamilies name = Harr);
void IDWT(cv::Mat input0, cv::Mat input1, cv::Mat output, const cv::Mat &lowfilter, const cv::Mat &highfilter);
void IDWT(cv::Mat input0, cv::Mat input1, cv::Mat output, WaveletFamilies name = Harr);
cv::Mat DWT2(cv::Mat input, int level, WaveletFamilies name = Harr);
cv::Mat IDWT2(cv::Mat input, int level, WaveletFamilies name = Harr);
void WaveletThreshold(cv::Mat input, float threshold, WaveletThresholdTpye type = Hard);
cv::Mat WaveletThresholdDenoise(cv::Mat input, int level, float threshold, WaveletFamilies name = Harr, WaveletThresholdTpye type = Hard);