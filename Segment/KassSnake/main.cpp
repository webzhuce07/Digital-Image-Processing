/**********************************************************************

Copyright (c) Mr.Bin. All rights reserved.
For more information visit: http://blog.csdn.net/webzhuce

**********************************************************************/
#include "kasssnake.h"

int main(int argc, char* argv[])
{
	cv::Mat src;
	int x0, y0, x1, y1, step;
	if (false)
	{
		src = cv::imread("./canon.jpg", 0);
		//cv::threshold(src, src, 90, 255, cv::THRESH_OTSU);
		x0 = 60; y0 = 90; x1 = 450; y1 = 640; step = 10;
	}
	else
	{
        src = cv::imread("../KassSnake/circle.bmp", 0);
		x0 = 10; y0 = 20; x1 = 230; y1 = 240; step = 10;
	}
		
	std::vector<cv::Point> points;
	for (int i = 0 ; i <= 400; i++)
	{
		float theta = 2 * 3.1415926 / 400 * i;
		int x = 130 + 110 * cos(theta);
		int y = 126 + 110 * sin(theta);
		points.push_back({ x, y });
	}
	////up side
	//for (int i = 0; i <= (x1 - x0) / step; i++)
	//{
	//	points.push_back({ x0 + i * step, y0 });
	//}
	////right side
	//for (int i = 1; i <= (y1 - y0) / step; i++)
	//{
	//	points.push_back({ x1, y0 + i * step});
	//}
	////bottom side
	//for (int i = (x1 - x0) / step - 1; i > 0 ; i--)
	//{
	//	points.push_back({ x0 + i * step, y1 });
	//}
	////left side
	//for (int i = (y1 - y0) / step; i > 0; i--)
	//{
	//	points.push_back({ x0, y0 + i * step });
	//}
	
	//SnakeImage snake(src);
	KassSnake snake(src);
	snake.segment(points);
	cv::waitKey(0);
	return EXIT_SUCCESS;
}

