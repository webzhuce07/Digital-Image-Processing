#include <opencv2/opencv.hpp>

using namespace cv;
void callback(int, void*);

int spatialRad = 50;  // 空间窗口半径
int colorRad = 50;   // 色彩距离
int maxPyrLevel = 2;  // 高斯金字塔层数

int main()
{
	Mat img = imread("D:\\TestData\\lena.jpg"); //读入图像  
	imshow("srcImg", img);

	// 拉动条
	namedWindow("param");
	createTrackbar("sr", "param", &spatialRad, 50, callback, (void *)&img);
	createTrackbar("cr", "param", &colorRad, 50, callback, (void *)&img);

	// 初始化
	callback(0, (void *)&img);

	// 等待退出
	waitKey(0);
	destroyAllWindows();

	return 0;
}

// 拖动条call函数
void callback(int, void* param) {
	Mat &img = *(Mat*)param; // 指针类变换
	Mat res; // 结果图
	pyrMeanShiftFiltering(img, res, spatialRad, colorRad, maxPyrLevel); // 均值漂移滤波

	// 显示
	imshow("Result", res);
}