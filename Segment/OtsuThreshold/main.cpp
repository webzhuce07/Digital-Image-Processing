#include <opencv2/opencv.hpp>
#include <iostream>
using namespace std;
using namespace cv;

double MyOtsu(const Mat& image, const uint graylevel = 256)
{
	CV_Assert(image.type() == CV_8UC1);

	//computer Histogram
	std::vector<long> histogram(graylevel, 0);
	for (int i = 0; i < image.rows; i++)
	{
		const uchar * p = image.ptr<uchar>(i);
		for (int j = 0; j < image.cols; j++)
			histogram[int(*p++)]++; 
	}

	long pixelsum = 0;
	uint gray = 0;
	for (gray = 0; gray < graylevel; gray++)
		pixelsum += gray * histogram[gray];

	long pixelnum = image.cols * image.rows;
	//compute a weighted sum of variances of two classes
	long w0 = 0, w1 = 0;
	double maxDelta = 0, tempDelta = 0, sum0 = 0, mu0 = 0, mu1 = 0;
	int  threshold = 0;
	for (gray = 0; gray < graylevel; gray++)
	{
		w0 += histogram[gray];
		w1 = pixelnum - w0;
		sum0 += gray * histogram[gray];
		if (w0 == 0 || w1 == 0)
			continue;
		mu0 = sum0 / w0;
		mu1 = (pixelsum - sum0) / w1;
		tempDelta = w0 * w1 * (mu0 - mu1) * (mu0 - mu1);
		if (tempDelta >= maxDelta)
		{
			maxDelta = tempDelta;
			threshold = gray;
		}
	}
	return threshold;
}

int main()
{
	Mat img = imread("../../ImageData/coins.png");
	Mat src;
	cvtColor(img, src, CV_BGR2GRAY);

	cout << "The return value of MyOstu is: " << MyOtsu(src) << endl;

	//call the Otsu method of OpenCV
	Mat dst;
	cout << "The return value of OpenCV threshold is: " << threshold(src, dst, 0, 255, CV_THRESH_OTSU) << endl;
	imshow("Threshold Segment", dst);
	waitKey();

	return 0;

}