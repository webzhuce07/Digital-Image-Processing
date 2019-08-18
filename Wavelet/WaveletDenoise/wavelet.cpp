#include "wavelet.h"

using namespace cv;

void WaveletFunction(cv::Mat &lowfilter, cv::Mat &highfilter, WaveletFamilies name /*= Harr*/)
{
	int n = 0;
	switch (name)
	{
	case Harr:
		n = 2;
		lowfilter = Mat::zeros(1, n, CV_32F);
		highfilter = Mat::zeros(1, n, CV_32F);
		lowfilter.at<float>(0, 0) = 0.7071;
		lowfilter.at<float>(0, 1) = 0.7071;
		highfilter.at<float>(0, 0) = -0.7071;
		highfilter.at<float>(0, 1) = 0.7071;
		break;
	case Symlets_2:
		n = 4;
		float h[] = { -0.483, 0.836, -0.224, -0.129 };
		float l[] = { -0.129, 0.224,    0.837, 0.483 };
		lowfilter = Mat::zeros(1, n, CV_32F);
		highfilter = Mat::zeros(1, n, CV_32F);
		for (int i = 0; i < n; i++)
		{
			lowfilter.at<float>(0, i) = l[i];
			highfilter.at<float>(0, i) = h[i];
		}
		break;
	}
}

void WaveletIFunction(cv::Mat &lowfilter, cv::Mat &highfilter, WaveletFamilies name /*= Harr*/)
{
	int n = 0;
	switch (name)
	{
	case Harr:
		n = 2;
		lowfilter = Mat::zeros(1, n, CV_32F);
		highfilter = Mat::zeros(1, n, CV_32F);
		lowfilter.at<float>(0, 0) = 0.7071;
		lowfilter.at<float>(0, 1) = 0.7071;
		highfilter.at<float>(0, 0) = 0.7071;
		highfilter.at<float>(0, 1) = -0.7071;
		break;
	case Symlets_2:
		n = 4;
		float h[] = { -0.1294, - 0.2241, 0.8365, - 0.4830 };
		float l[] = { 0.4830, 0.8365, 0.2241, - 0.1294 };
		lowfilter = Mat::zeros(1, n, CV_32F);
		highfilter = Mat::zeros(1, n, CV_32F);
		for (int i = 0; i < n; i++)
		{
			lowfilter.at<float>(0, i) = l[i];
			highfilter.at<float>(0, i) = h[i];
		}
		break;
	}

}

//one dimension
void DWT(const cv::Mat input, cv::Mat output, const cv::Mat &lowfilter, const cv::Mat &highfilter)
{
	assert(input.rows == 1 || input.cols == 1);
	if (input.rows == 1) //horizontal
	{
		assert(lowfilter.rows == 1 && highfilter.rows == 1);
		assert(input.cols >= lowfilter.cols && input.cols >= highfilter.cols);
		Mat dst1, dst2;
		input.copyTo(dst1);
		input.copyTo(dst2);
		filter2D(input, dst1, -1, lowfilter);
		filter2D(input, dst2, -1, highfilter);

		//down sample
		for (int i = 0, j = 1; i < input.cols / 2; i++, j += 2)
		{
			output.at<float>(0, i) = dst1.at<float>(0, j);
			output.at<float>(0, i + input.cols / 2) = dst2.at<float>(0, j);
		}
	}
	else
	{
		assert(lowfilter.cols == 1 && highfilter.cols == 1);
		assert(input.rows >= lowfilter.rows && input.rows >= highfilter.rows);
		Mat dst1, dst2;
		input.copyTo(dst1);
		input.copyTo(dst2);
		filter2D(input, dst1, -1, lowfilter);
		filter2D(input, dst2, -1, highfilter);

		//down sample
		for (int i = 0, j = 1; i < input.rows / 2; i++, j += 2)
		{
			output.at<float>(i, 0) = dst1.at<float>(j, 0);
			output.at<float>(i + input.rows / 2, 0) = dst2.at<float>(j,0);
		}
	}
}

void DWT(const cv::Mat input, cv::Mat output, WaveletFamilies name /*= Harr*/)
{
	Mat lowfilter, highfilter;
	WaveletFunction(lowfilter, highfilter, name);
	DWT(input, output, lowfilter, highfilter);

}
//input0：分解后的近似部分序列（低频部分），input1：分解后的细节部分序列（高频部分）
void IDWT(cv::Mat input0, cv::Mat input1, cv::Mat output, const cv::Mat &lowfilter, const cv::Mat &highfilter)
{
	assert(input0.rows == 1 || input0.cols == 1);
	assert(input1.rows == 1 || input1.cols == 1);
	assert(input0.rows == input1.rows || input0.cols == input1.cols);

	if (input0.rows == 1) //horizontal
	{
		assert(lowfilter.rows == 1 && highfilter.rows == 1);
		assert(input0.cols >= lowfilter.cols && input0.cols >= highfilter.cols);
		assert(input1.cols >= lowfilter.cols && input1.cols >= highfilter.cols);
		//up sample
		Mat updst0 = Mat::ones(output.size(), output.type());
		Mat updst1 = Mat::ones(output.size(), output.type());
		for (int i = 0, j = 0; i < output.cols / 2; i++, j += 2)
		{
			updst0.at<float>(0, j) = input0.at<float>(0, i);
			updst1.at<float>(0, j) = input1.at<float>(0, i);
		}

		filter2D(updst0, updst0, -1, lowfilter);
		filter2D(updst1, updst1, -1, highfilter);
		output = updst1 + updst0;
	}
	else
	{
		assert(lowfilter.cols == 1 && highfilter.cols == 1);
		assert(input0.rows >= lowfilter.rows && input0.rows >= highfilter.rows);
		assert(input1.rows >= lowfilter.rows && input1.rows >= highfilter.rows);
		//up sample
		Mat updst0 = Mat::ones(output.size(), output.type());
		Mat updst1 = Mat::ones(output.size(), output.type());
		for (int i = 0, j = 0; i < output.rows / 2; i++, j += 2)
		{
			updst0.at<float>(j, 0) = input0.at<float>(i, 0);
			updst1.at<float>(j, 0) = input1.at<float>(i, 0);
		}

		filter2D(updst0, updst0, -1, lowfilter);
		filter2D(updst1, updst1, -1, highfilter);
		output = updst1 + updst0;
	}
	
}

void IDWT(cv::Mat input0, cv::Mat input1, cv::Mat output, WaveletFamilies name /*= Harr*/)
{
	Mat lowfilter, highfilter;
	WaveletIFunction(lowfilter, highfilter, name);
	IDWT(input0, input1,output, lowfilter, highfilter);
}

cv::Mat DWT2(cv::Mat input, int level, WaveletFamilies name /*= Harr*/)
{
	Mat output;
	input.copyTo(output);
	output.convertTo(output, CV_32FC1);
	int rows = input.rows, cols = input.cols;
	Mat lowfilter, highfilter;
	WaveletFunction(lowfilter, highfilter, name);
	int t = 1;
	Rect rowrect(0, 0, input.cols, 1);
	Rect colmunrect(0, 0, 1, input.rows);
	Mat roi;
	while (t <= level)
	{
		rowrect.width = cols;
		colmunrect.height = rows;
		//first: row transform
		for (int i = 0; i < rows; i++)
		{
			rowrect.y = i;
			roi = output(rowrect);
			DWT(roi, roi, lowfilter, highfilter);
		}
		
		//second: column transform
		for (int j = 0; j < cols; j++)
		{
			colmunrect.x = j;
			roi = output(colmunrect);
			DWT(roi, roi, lowfilter.t(), highfilter.t());
		}

		rows /= 2;
		cols /= 2;
		++t;
	}
	return output;
}

cv::Mat IDWT2(cv::Mat input, int level,  WaveletFamilies name /*= Harr*/)
{
	Mat output;
	input.copyTo(output);
	output.convertTo(output, CV_32FC1);
	Mat lowfilter, highfilter;
	WaveletIFunction(lowfilter, highfilter, name);
	Rect rowrect(0, 0, input.cols, 1);
	Rect colmunrect(0, 0, 1, input.rows);
	Mat input0 , input1, roi;
	int t = level - 1, rows= 0, cols = 0;
	while (t >= 0)
	{
		rows = input.rows / std::pow(2, t);
		cols = input.cols / std::pow(2, t);

		//first: column transform
		for (int i = 0; i < cols; i++)
		{
			colmunrect.x = i;
			colmunrect.y = 0;
			colmunrect.height = rows / 2;
			input0 = output(colmunrect);
			colmunrect.y = rows / 2;
			colmunrect.height = rows - rows / 2;
			input1 = output(colmunrect);
			colmunrect.y = 0;
			colmunrect.height = rows;
			roi = output(colmunrect);
			IDWT(input0, input1, roi, lowfilter.t(), highfilter.t());
		}

		//second: row transform
		for (int j = 0; j < rows; j++)
		{
			rowrect.y = j;
			rowrect.x = 0;
			rowrect.width = cols / 2;
			input0 = output(rowrect);
			rowrect.x = cols / 2;
			rowrect.width = cols - cols / 2;
			input1 = output(rowrect);
			rowrect.x = 0;
			rowrect.width = cols;
			roi = output(rowrect);
			IDWT(input0, input1, roi, lowfilter, highfilter);
		}
		--t;
	}

	output.convertTo(output, CV_8UC1);
	return output;
}

void WaveletThreshold(cv::Mat input, float threshold, WaveletThresholdTpye type /*= Hard*/)
{
	switch (type)
	{
	case Hard:
		for (int i = 0; i < input.rows; i++)
		{
			for (int j = 0; j < input.cols; j++)
			{
				if (abs(input.at<float>(i, j)) > threshold)
					continue;
				input.at<float>(i, j) = 0.0;
			}
		}
		break;
	case Soft:
		for (int i = 0; i < input.rows; i++)
		{
			for (int j = 0; j < input.cols; j++)
			{
				if (abs(input.at<float>(i, j)) > threshold)
				{
					if (input.at<float>(i, j) < 0)
					{
						input.at<float>(i, j) = threshold + input.at<float>(i, j);
					}
					else
					{
						input.at<float>(i, j) = input.at<float>(i, j) - threshold;
					}
				}
				else
				{
					input.at<float>(i, j) = 0.0;
				}				
			}
		}
		break;
	default:
		break;
	}

}

cv::Mat WaveletThresholdDenoise(cv::Mat input, int level, float threshold, WaveletFamilies name /*= Harr*/, WaveletThresholdTpye type /*= Hard*/)
{
	 Mat output = DWT2(input, level, name);
	//ThresholdDenoise
	int t = 1, rows = output.rows, cols = output.cols;
	Rect rect;
	Mat roi;
	while (t <= level)
	{
		//left-down
		rect.x = 0;
		rect.y = rows / 2;
		rect.width = cols / 2;
		rect.height = rows - rows / 2;
		roi = output(rect);
		WaveletThreshold(roi, threshold,type);

		//right-down
		rect.x = cols / 2;
		rect.width = cols - cols / 2;
		roi = output(rect);
		WaveletThreshold(roi, threshold, type);

		//right-up
		rect.y = 0;
		rect.height = rows / 2;
		roi = output(rect);
		WaveletThreshold(roi, threshold, type);

		rows /= 2;
		cols /= 2;
		++t;
	}
	output = IDWT2(output, level, name);
	output.convertTo(output, CV_8UC1);
	return output;
}

