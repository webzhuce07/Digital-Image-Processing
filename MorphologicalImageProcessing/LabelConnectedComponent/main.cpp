/**********************************************************************

Copyright (c) Mr.Bin. All rights reserved.
For more information visit: http://blog.csdn.net/webzhuce

**********************************************************************/
#include<opencv2/opencv.hpp>
using namespace cv;


/*
 *only process binary image
 *iseight = true means eight connected, otherwise four connected
*/
int LabelConnectedComponent(const cv::Mat &src, cv::Mat &dst, bool iseight = true)
{
    Mat structelement;
    if(iseight)
        structelement = getStructuringElement(MORPH_RECT, Size(3, 3));
    else
         structelement = getStructuringElement(MORPH_CROSS, Size(3, 3));

    dst = Mat::ones(src.size(), src.type());
    Mat tmp = Mat::ones(src.size(), src.type()); // save last reuslt image
    Mat img = Mat::ones(src.size(), src.type()); //image B
    int labelnum = 0; //label of connected component

    Mat backupsrc;
    src.copyTo(backupsrc);
    for(int i = 0; i < backupsrc.rows; i++)
    {
        for(int j = 0; j < backupsrc.cols; j++)
        {
            if(backupsrc.at<uchar>(i, j) == 255)
            {
                Mat img = Mat::ones(src.size(), src.type());
                img.at<uchar>(i, j) = 255;
                img.copyTo(tmp);  //Temporary save
                labelnum++;

                while(true)  // until not change
                {
                    cv::dilate(img, img, structelement);
                    bitwise_and(img, src, img);
                    //if img do not change, this connected component is finished
                    if (cv::countNonZero(img - tmp) == 0)
                        break;
                    img.copyTo(tmp);
                }


                //label the connected component
                for(int r = 0; r < img.rows; r++)
                {
                    for(int c = 0; c < img.cols; c++)
                    {
                        if(img.at<uchar>(r, c) == 255)
                        {
                            backupsrc.at<uchar>(r, c) = 0;
                            dst.at<uchar>(r, c) = labelnum;
                        }
                    }
                }
            }
        }
    }
    return labelnum;
}

int main(int argc, char *argv[])
{
   Mat originimage = imread("../LabelConnectedComponent/bw_bacteria.bmp", 0);
   Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));
   imshow("OriginImage", originimage);
   Mat dilateimage;
   cv::dilate(originimage, dilateimage, element);
   imshow("DilateImage", dilateimage);

   Mat output;
   int num = LabelConnectedComponent(dilateimage, output);
   std::cout<< num << std::endl;
   imshow("Output", output);


    waitKey(0);
    return 0;

}
