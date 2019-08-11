/**********************************************************************

Copyright (c) Mr.Bin. All rights reserved.
For more information visit: http://blog.csdn.net/webzhuce

**********************************************************************/
#include "graphcut.h"

using namespace  cv;

Mat g_image;
Mat g_currentimage;
std::vector<cv::Point> g_objectseeds;
std::vector<cv::Point> g_backgroundseeds;
const char *g_kOrigin = "Origin";
const char *g_kResult = "Result";

void SelectSeed(int x, int y, bool object = true)
{
	const int kRadius = 5;
	cv::Scalar color = object ? Scalar(0, 255, 0) : Scalar(255, 0, 0);
	for (int i = -kRadius / 2; i <= kRadius / 2; i++)
	{
		int px = x + i;
		if (px < 0 || px >= g_image.cols)
			continue;
		for (int j = -kRadius / 2; j <= kRadius / 2; j++)
		{
			int py = y + j;
			if (py < 0 || py >= g_image.rows)
				continue;
			cv::Point p(px, py);
			if (object)
				g_objectseeds.push_back(p);
			else
				g_backgroundseeds.push_back(p);
		}
	}
	cv::circle(g_currentimage, cv::Point(x, y), kRadius, color, -1);
}

//left button select object(green), right button select background (blue).
void MouseFunction(int mEvent, int x, int y, int flags, void* p)
{
	switch (mEvent)
	{
	case CV_EVENT_LBUTTONDOWN://
	{
		SelectSeed(x, y, true);
		break;
	}
	case CV_EVENT_RBUTTONDOWN:
	{
		SelectSeed(x, y, false);
		break;
	}
	case CV_EVENT_MOUSEMOVE:
	{
		if (flags == CV_EVENT_FLAG_LBUTTON)
			SelectSeed(x, y, true);
		if (flags == CV_EVENT_FLAG_RBUTTON)
			SelectSeed(x, y, false);
		break;
	}
	case CV_EVENT_MBUTTONDOWN:
	{
		g_backgroundseeds.clear();
		g_objectseeds.clear();
		g_image.copyTo(g_currentimage);
		break;
	}
	default:
		break;
	}
	imshow(g_kOrigin, g_currentimage);
}

int main()
{
	g_image = imread("./cut.jpg", IMREAD_COLOR); // indicate 3 channels
	g_image.copyTo(g_currentimage);
	imshow(g_kOrigin, g_currentimage);
	setMouseCallback(g_kOrigin, MouseFunction);
	//press enter key to cut
	while (waitKey() != 13 || g_objectseeds.empty() || g_backgroundseeds.empty())
	{
		//waiting
	} 
	std::cout << "Starting graph cut..." << std::endl;
	GraphCut cut(g_image);
	Mat result = cut.runInitially(g_objectseeds, g_backgroundseeds);
	imshow(g_kResult, result);
	waitKey();
	imwrite("./result.png", result);
	system("pause");

	return EXIT_SUCCESS;
}