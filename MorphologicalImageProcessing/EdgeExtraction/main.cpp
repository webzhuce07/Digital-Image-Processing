/**********************************************************************

Copyright (c) Mr.Bin. All rights reserved.
For more information visit: http://blog.csdn.net/webzhuce

**********************************************************************/
#include<opencv2/opencv.hpp>
using namespace cv;

int main(int argc, char *argv[])
{
   Mat originimage = imread("../EdgeExtraction/head_portrait.bmp");
   Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));
   imshow("OriginImage", originimage);
   Mat erodeimage;
   cv::erode(originimage, erodeimage, element);
   imshow("ErodeImage", erodeimage);

   Mat output = originimage -  erodeimage;
   imshow("Output", output);


    waitKey(0);
    return 0;

}
