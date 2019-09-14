/**********************************************************************

Copyright (c) Mr.Bin. All rights reserved.
For more information visit: http://blog.csdn.net/webzhuce

**********************************************************************/
#include "snakeimage.h"

int main(int argc, char* argv[])
{
	cv::Mat src;
	int x0, y0, x1, y1, step;
	if (false)
	{
        src = cv::imread("../CvSnakeImage/canon.jpg", 0);
		cv::threshold(src, src, 90, 255, cv::THRESH_OTSU);
		x0 = 60; y0 = 90; x1 = 450; y1 = 640; step = 10;
	}
	else
	{
        src = cv::imread("../CvSnakeImage/circle.bmp", 0);
		x0 = 10; y0 = 20; x1 = 230; y1 = 240; step = 10;
	}
		
	std::vector<cv::Point> points;
	//up side
	for (int i = 0; i <= (x1 - x0) / step; i++)
	{
		points.push_back({ x0 + i * step, y0 });
	}
	//right side
	for (int i = 1; i <= (y1 - y0) / step; i++)
	{
		points.push_back({ x1, y0 + i * step});
	}
	//bottom side
	for (int i = (x1 - x0) / step - 1; i > 0 ; i--)
	{
		points.push_back({ x0 + i * step, y1 });
	}
	//left side
	for (int i = (y1 - y0) / step; i > 0; i--)
	{
		points.push_back({ x0, y0 + i * step });
	}
	
	SnakeImage snake(src);
	snake.segment(points);
	cv::waitKey(0);
	return EXIT_SUCCESS;
}

