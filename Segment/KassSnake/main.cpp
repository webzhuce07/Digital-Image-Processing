/**********************************************************************

Copyright (c) Mr.Bin. All rights reserved.
For more information visit: http://blog.csdn.net/webzhuce

**********************************************************************/
#include "kasssnake.h"

int main(int argc, char* argv[])
{
    cv::Mat src = cv::imread("../KassSnake/circle.jpg", 0);
	std::vector<cv::Point2f> points;
	const int num = 100;
	for (int i = 0 ; i < num; i++)
	{
		float theta = 2 * 3.1415926 / num * i;
		float x = 130 + 110 * cos(theta);
		float y = 126 + 110 * sin(theta);
		points.push_back({ x, y });
	}
	KassSnake snake(src);
	snake.segment(points);
	cv::waitKey(0);
	return EXIT_SUCCESS;
}

