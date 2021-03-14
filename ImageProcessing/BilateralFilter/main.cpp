#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;


int g_d = 15;
int g_sigmaColor = 20;
int g_sigmaSpace = 50;

Mat g_srcimg;
Mat g_dstimg;

void on_Trackbar(int, void*)
{
	bilateralFilter(g_srcimg, g_dstimg, g_d, g_sigmaColor, g_sigmaSpace);
	imshow("output", g_dstimg);
}


int main(int argc, char **argv)
{
	g_srcimg = imread("D:\\TestData\\lena.jpg");
	if (g_srcimg.empty())
	{
		cout << "Could not load image ... " << endl;
		return  -1;
	}

	g_dstimg = Mat::zeros(g_srcimg.rows, g_srcimg.cols, g_srcimg.type());
	bilateralFilter(g_srcimg, g_dstimg, g_d, g_sigmaColor, g_sigmaSpace);

	namedWindow("input");

	createTrackbar("核直径", "input", &g_d, 50, on_Trackbar);
	createTrackbar("颜色空间方差", "input", &g_sigmaColor, 100, on_Trackbar);
	createTrackbar("坐标空间方差", "input", &g_sigmaSpace, 100, on_Trackbar);

	imshow("input", g_srcimg);
	imshow("output", g_dstimg);

	waitKey(0);
	return 0;
}