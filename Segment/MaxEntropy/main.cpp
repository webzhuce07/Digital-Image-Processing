#include <iostream>
#include <opencv2/opencv.hpp>

const double g_EPSINON = 0.00001;
int MaxEntropy(const cv::Mat& src, cv::Mat& dst, int thresh, int factor)
{
	const int k_Grayscale = 256;
	double hist[k_Grayscale] = { 0.0 };
	int r = src.rows;
	int c = src.cols;
	for (int i = 0; i < r; ++i)
	{
		const uchar* ptr = src.ptr<uchar>(i);   
		for (int j = 0; j < c; ++j)
			hist[ptr[j]]++;
	}
 
	double probability = 0.0; //概率
	double maxEntropy = DBL_MIN; //最大熵
	int totalpix = r * c;
	for (int i = 0; i < k_Grayscale; ++i)
	{ 
	    //计算背景像素数
		double backgroundpix = 0;
		for (int j = 0; j <= i; ++j)
			backgroundpix += hist[j];
		//计算背景熵
		double H0 = 0.0; //背景熵
		if (backgroundpix > 0)
		{
			for (int j = 0; j <= i; ++j)
			{
				probability = hist[j] / backgroundpix;
				if(probability > g_EPSINON)
					H0 += -probability * log(probability);
			}
		}
		
		//计算前景像素数
		double frontpix = totalpix - backgroundpix;
		//计算前景熵
		double H1 = 0.0; //前景熵
		if (frontpix > 0)
		{
			for (int k = i + 1; k < k_Grayscale; ++k)
			{
				probability = hist[k] / frontpix;
				if (probability > g_EPSINON)
					H1 += -probability * log(probability);
			}
		}
		
		//计算最大熵
		if(H0 + H1 > maxEntropy)
		{
			maxEntropy = H0 + H1;
			thresh = i;
		}
	}
 
	thresh += factor;
	//阈值处理
	src.copyTo(dst);
	for (int i = 0; i < r; ++i){
		uchar* ptr = dst.ptr<uchar>(i);
		for (int j = 0; j < c; ++j){
			if (ptr[j]> thresh)
				ptr[j] = 255;
			else
				ptr[j] = 0;
		}
	}
	return thresh;
}


void MakeTable(double *S0, double *S1, double *normalizeHist, int histSize)
{
	double s0 = 0.0;
	for (int i = 0; i < histSize; i++)
	{	
		if(normalizeHist[i] > g_EPSINON)
			s0 = s0 + normalizeHist[i] * log(normalizeHist[i]);
		S0[i] = s0;
	}

	double s1 = 0.0;
	for (int i = histSize - 1; i >= 0; i--)
	{
		S1[i] = s1;
		if (normalizeHist[i] > g_EPSINON)
			s1 = s1 + normalizeHist[i] * log(normalizeHist[i]);
		
	}
}

int MaxEntropyFast(const cv::Mat& src, cv::Mat& dst, int thresh, int factor)
{
	const int k_GrayScale = 256;
	int hist[k_GrayScale] = { 0 };
	int r = src.rows;
	int c = src.cols;
	for (int i = 0; i < r; ++i) {
		const uchar* ptr = src.ptr<uchar>(i);
		for (int j = 0; j < c; ++j) 
			hist[ptr[j]]++;
	}

	double normalizeHist[256] = { 0.0 }, sum = (double)(r * c);
	for (int i = 0; i < k_GrayScale; i++)
		normalizeHist[i] = hist[i] / sum ;

	double S0[256] = { 0 }, S1[256] = { 0 };	
	MakeTable(S0, S1, normalizeHist, k_GrayScale);

	double H01 = 0.0, H0 = 0.0, H1 = 0.0, P1 = 0.0, P0 = 0.0, Hmax = DBL_MIN;
	for (int q = 0; q < k_GrayScale; q++)
	{
		P0 += normalizeHist[q];
		if (P0 > g_EPSINON)
			H0 = -(1.0 * S0[q]) / P0 + log(P0);
		else
			H0 = 0.0;

		P1 = 1.0 - P0;
		if (P1 > g_EPSINON)
			H1 = -(1.0 * S1[q]) / P1 + log(P1);
		else
			H1 = 0.0;

		H01 = H0 + H1;

		if (H01 > Hmax)
		{
			Hmax = H01;
			thresh = q;
		}

	}

	thresh += factor;
	//阈值处理
	src.copyTo(dst);
	for (int i = 0; i < r; ++i) {
		uchar* ptr = dst.ptr<uchar>(i);
		for (int j = 0; j < c; ++j) {
			if (ptr[j] > thresh)
				ptr[j] = 255;
			else
				ptr[j] = 0;
		}
	}
	return thresh;
}
 
 
int main()
{
	cv::Mat src = cv::imread("cameraman.tif");
	//cv::Mat src = cv::imread("test120.png");
	if (src.empty()){
		return -1;
	}
	if (src.channels() > 1)
		cv::cvtColor(src, src, CV_RGB2GRAY);
 
	cv::Mat dst, dstOtsu;
	int thresh = 0;
	double t2 = (double)cv::getTickCount();
	thresh = MaxEntropy(src, dst, thresh, 0); //Max_Entropy
	std::cout << "MaxEntropyThresh=" << thresh << std::endl;
	t2 = (double)cv::getTickCount() - t2;
	double time2 = (t2 *1000.) / ((double)cv::getTickFrequency());
	std::cout << "MaxEntropyProcess=" << time2 << " ms. " << std::endl << std::endl;

	cv::Mat dstFast;
	thresh = 0;
	t2 = (double)cv::getTickCount();
	thresh = MaxEntropyFast(src, dstFast, thresh, 0); //Max_Entropy
	std::cout << "MaxEntropyFastThresh=" << thresh << std::endl;
	t2 = (double)cv::getTickCount() - t2;
	time2 = (t2 *1000.) / ((double)cv::getTickFrequency());
	std::cout << "MaxEntropyFastProcess=" << time2 << " ms. " << std::endl << std::endl;

 
	double  Otsu = 0;
	Otsu = cv::threshold(src, dstOtsu, Otsu, 255, CV_THRESH_OTSU + CV_THRESH_BINARY);
	std::cout << "OtsuThresh=" << Otsu << std::endl;
	
 
	cv::namedWindow("src", CV_WINDOW_NORMAL);
	cv::imshow("src", src);
	cv::namedWindow("dst", CV_WINDOW_NORMAL);
	cv::imshow("dst", dst);
	cv::namedWindow("dstOtsu", CV_WINDOW_NORMAL);
	cv::imshow("dstOtsu", dstOtsu);
	//cv::imwrite("r.jpg",dst);
	cv::waitKey(0);
}