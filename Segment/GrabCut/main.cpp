/**********************************************************************

Copyright (c) Mr.Bin. All rights reserved.
For more information visit: http://blog.csdn.net/webzhuce

**********************************************************************/
#include<opencv2\opencv.hpp>
using namespace cv;

struct GrabCutData
{
	Mat src;
	Rect rect;
};

void onMouse(int event, int x, int y, int flags, void *userdata)
{
	GrabCutData *data = (GrabCutData*)userdata;
	switch (event)
	{
	case CV_EVENT_LBUTTONDOWN:
		data->rect = Rect(x, y, 1, 1);
		break;
	case CV_EVENT_MOUSEMOVE:
		if (flags && CV_EVENT_FLAG_LBUTTON)
		{
			data->rect = Rect(Point(data->rect.x, data->rect.y), Point(x, y));
			Mat image;
			data->src.copyTo(image);
			rectangle(image, data->rect, Scalar(0, 0, 255), 2);
			imshow("input", image);
		}
		break;
	case EVENT_RBUTTONDOWN:
		if (data->rect.width > 1 && data->rect.height > 1)
		{
			//result：GC_BGD =0（背景）,GC_FGD =1（前景）,GC_PR_BGD = 2（可能的背景）, GC_PR_FGD = 3（可能的前景）		
			Mat result = Mat::zeros(data->src.size(), CV_8UC1);
			// GrabCut 
			Mat bgModel, fgModel;
			grabCut(data->src, result, data->rect, bgModel, fgModel, 1, GC_INIT_WITH_RECT);
			//set GC_FGD to result
			compare(result, GC_PR_FGD, result, CMP_EQ);
			// output image
			Mat foreground(data->src.size(), CV_8UC3, Scalar(255, 255, 255));
			data->src.copyTo(foreground, result);
			imshow("output", foreground);
		}
		break;
	default:
		break;
	}
}

int main(int arc, char** argv) 
{
	GrabCutData data;
	data.src = imread("test.jpg");
	namedWindow("input", CV_WINDOW_AUTOSIZE);
	imshow("input", data.src);
	setMouseCallback("input", onMouse, &data);
	waitKey(0);
	return 0;
}

