/**********************************************************************

Copyright (c) Mr.Bin. All rights reserved.
For more information visit: http://blog.csdn.net/webzhuce

**********************************************************************/
#include "levelset.h"

int main(int argc, char* argv[])
{
	cv::Mat src;
	if (true)
	{
        src = cv::imread("../LevelSet/circle.bmp");
		LevelSet levelset(src);
		levelset.initializePhi({ 130.0f, 130.0f }, 50.0f);
		levelset.evolving();
	}
	else
	{
        src = cv::imread("../LevelSet/mouth.png");
		LevelSet levelset(src);
		levelset.initializePhi({ 400.0f, 240.0f }, 200.0f);
		levelset.evolving();
	}

	cv::waitKey(0);
	return EXIT_SUCCESS;
}

