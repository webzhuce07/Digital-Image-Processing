/**********************************************************************

Copyright (c) Mr.Bin. All rights reserved.
For more information visit: http://blog.csdn.net/webzhuce

**********************************************************************/
#include<opencv2/opencv.hpp>
using namespace cv;

/**********************************************
功能：3*3结构元素的二值图像的腐蚀
参数：src-输入图像
     dst-输出图像
     se-模板
***********************************************/
void  CustomErode(const Mat &src, Mat &dst,int se[3][3])
{
    if (src.empty())
    {
        return;
    }
    dst = Mat::zeros(src.size(), src.type());
    bool match = true; //结构元素是否与局部图像匹配
    //逐行扫描图像，为防止越界，四周留出一个像素的空边
    for (int i = 1; i < src.rows - 1; i++)
    {
        uchar* data = dst.ptr<uchar>(i);
        for (int j = 1; j < src.cols - 1; j++)
        {
            match = true;
            for (int m = 0; m < 3; m++)
            {
                for (int n = 0; n < 3; n++)
                {
                    if (se[m][n] == -1)//不关心
                        continue;
                    if (se[m][n] == 1) //前景
                    {
                        if (src.at<uchar>(i - 1 + m, j - 1 + n) != 255)
                        {
                            match = false;
                            break;
                        }
                    }
                    else if (se[m][n]==0)//背景
                    {
                        if (src.at<uchar>(i - 1 + m, j - 1 + n) != 0)
                        {
                            match = false;
                            break;
                        }
                    }
                    else
                    {
                        std::cout << "模板结构元素错误！！！" << std::endl;
                        return;
                    }
                } //for n
            } // for m
            if (match)
            {
                data[j] = 255;
            }
        } // for j
    } //for i
}

/*
 *only process binary image
 * white is foreground
*/
void Convex(const cv::Mat &src, cv::Mat &dst, bool constrain)
{
    //four struct elements to compute convex
    int se1[3][3] = {{1,-1,-1},
                     {1,0,-1},
                     {1,-1,1}};  //弥补右侧的凸缺
    int se2[3][3] = {{1,1,1},
                     {-1,0,-1},
                     {-1,-1,-1}}; //弥补下侧的凸缺
    int se3[3][3] = {{-1,-1,1},
                     {-1,0,1},
                     {-1,-1,1}}; //弥补左侧的凸缺
    int se4[3][3] = {{-1,-1,-1},
                     {-1,0,-1},
                     {1,1,1}}; //弥补上侧的凸缺

     src.copyTo(dst);
    //采用第一个结构元素
    Mat backupsrc1;
    src.copyTo(backupsrc1);
    while(true)
    {
        CustomErode(backupsrc1, dst, se1);
        bitwise_or(dst, backupsrc1, dst);
        if(cv::countNonZero(dst - backupsrc1) == 0)
            break;
        dst.copyTo(backupsrc1);
    }

    //采用第二个结构元素
    Mat backupsrc2;
    src.copyTo(backupsrc2);
    while(true)
    {
        CustomErode(backupsrc2, dst, se2);
        bitwise_or(dst, backupsrc2, dst);
        if(cv::countNonZero(dst - backupsrc2) == 0)
            break;
        dst.copyTo(backupsrc2);
    }

    //采用第三个结构元素
    Mat backupsrc3;
    src.copyTo(backupsrc3);
    while(true)
    {
        CustomErode(backupsrc3, dst, se3);
        bitwise_or(dst, backupsrc3, dst);
        if(cv::countNonZero(dst - backupsrc3) == 0)
            break;
        dst.copyTo(backupsrc3);
    }

    //采用第四个结构元素
    Mat backupsrc4;
    src.copyTo(backupsrc4);
    while(true)
    {
        CustomErode(backupsrc4, dst, se4);
        bitwise_or(dst, backupsrc4, dst);
        if(cv::countNonZero(dst - backupsrc4) == 0)
            break;
        dst.copyTo(backupsrc4);
    }

    dst = Mat::zeros(src.size(), src.type());
    for(int i = 0; i < src.rows; i++)
    {
        for(int j = 1; j < src.cols; j++)
        {
            if(backupsrc1.at<uchar>(i, j) == 255 || backupsrc2.at<uchar>(i, j) == 255 ||
                    backupsrc3.at<uchar>(i, j) == 255 ||backupsrc4.at<uchar>(i, j) == 255)
                dst.at<uchar>(i, j) = 255;
        }
    }

    //需要限制凸壳的生长
    //找到原始图像中物体的范围
    if(constrain)
    {
        int top = src.rows;
        int bottom = 0;
        int left = src.cols;
        int right = 0;
        for(int i = 0; i < src.rows; i++)
        {
            for(int j = 1; j < src.cols; j++)
            {
                if(src.at<uchar>(i, j) == 0) // background
                    continue;
                if(i < top)
                    top = i;
                if(i > bottom)
                    bottom = i;
                if(j < left)
                    left = j;
                if(j > right)
                    right = j;
            }
        }
        for(int i = 0; i < src.rows; i++)
        {
            for(int j = 1; j < src.cols; j++)
            {
                if( i < top ||  i > bottom || j < left || j > right)
                    dst.at<uchar>(i, j) = 0;
            }
        }
    }
}

int main(int argc, char *argv[])
{
   Mat originimage = imread("../Convex/bw_mouth_VC.bmp", 0);
   imshow("OriginImage", originimage);
   //white is foreground
   Mat image;
   bitwise_not(originimage, image);
   imshow("Image", image);

   Mat output;
   Convex(image, output, true);
   bitwise_not(output, output);
   imshow("Output", output);

    waitKey(0);
    return 0;

}
