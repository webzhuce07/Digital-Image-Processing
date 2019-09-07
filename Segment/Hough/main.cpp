/**********************************************************************

Copyright (c) Mr.Bin. All rights reserved.
For more information visit: http://blog.csdn.net/webzhuce

**********************************************************************/
#include<opencv2/opencv.hpp>
#include<stdio.h>
using namespace cv;
using namespace std;

/**********************************************
功能：霍夫变换
参数：src-输入图像
     lines-直线信息
     linenum-寻找的直线数目
***********************************************/
void  CumstomHough(const Mat &src, std::vector<cv::Vec2f> &lines, int linenum = 3)
{
    //极坐标中的最大rho和theta
    int maxdist = std::sqrt(std::pow(src.rows, 2)+ std::pow(src.cols, 2));
    int maxangle = 180;
    //为极坐标分配空间
    int areanum =  maxangle * maxdist * 2;
    int *transarea = new int[areanum];
    memset(transarea, 0, areanum * sizeof(int));

    //转换到极坐标
    uchar pixel = 0;
    int angle =0, dist = 0; //极坐标下角度和极径
    double radian = 0; // 弧度

    for(int i = 0; i < src.rows; i++)
    {
        for(int j = 0; j < src.cols; j++)
        {
             pixel = src.at<uchar>(i, j);
             if(pixel == 255)
             {
                for(angle = 0; angle < maxangle; angle++)
                {
                    radian = angle * 3.1415926 / 180.0; //转换为弧度
                    dist = j * cos(radian) + i * sin(radian);
                    if(dist >= 0) //正半周
                        transarea[dist*maxangle + angle]++;
                    else //负半周
                    {
                        dist = std::abs(dist);
                        transarea[maxangle * maxdist + dist * maxangle + angle]++;
                    }
                } //for angle
             } // if
        } // for j
    } //for i

    //清零时角度和极径的范围
    int maxdistallow = 20, maxangleallow = 5;
    for(int n = 0; n < linenum; n++) //寻找前line个峰值点
    {
        dist = 0; //寻找最大点
        for(int i = 0; i < areanum; i++)
        {
            if(transarea[i] > dist)
            {
                dist = transarea[i];
                angle = i;
            }
        }
        //找不到可能的共线点
        if(dist == 0)
            return;
        if(angle < maxangle * maxdist)
        {
            dist =  angle / maxangle;
            angle = angle % maxangle;
        }
        else
        {
            angle -= maxangle * maxdist;
            dist = angle / maxangle;
            dist *= -1;
            angle = angle % maxangle;
        }
        //保存结果
        lines.push_back({float(dist), float(angle)});


        //将附近点清零，为寻找下个峰值做准备
        for(dist = -1 * maxdistallow; dist <= maxdistallow; dist++)
        {
            for(angle = -1*maxangleallow; angle <= maxangleallow; angle++)
            {
                int thisdist = lines[n][0] + dist;
                int thisangle = lines[n][1] + angle;
                if(thisangle < 0 && thisangle >= -180)
                {
                    thisangle += 180;
                    thisdist *= -1;
                }
                if(thisangle >= 180 && thisangle < 360)
                {
                    thisangle -= 180;
                     thisdist *= -1;
                }
                if(std::abs(thisdist) <= maxdist && thisangle >= 0 && thisangle <= maxangle)
                {
                    if(thisdist >= 0)
                    {
                        transarea[thisdist * maxangle + thisangle] = 0;
                    }
                    else
                    {
                        thisdist = std::abs(thisdist);
                        transarea[maxdist * maxangle + thisdist * maxangle + thisangle] = 0;
                    }
                }
            } //for angle
        }  //for dist

    }  //for n

    delete[] transarea;
}

int main(int argc, char *argv[])
{
   Mat src = imread("../Hough/line.bmp", 1);
   const Mat gray = imread("../Hough/line.bmp", 0);
   std::vector<Vec2f> lines;//定义一个矢量结构lines用于存放得到的线段矢量集合
   HoughLines(gray, lines, 1, CV_PI / 180, 150, 0, 0);
   //依次在图中绘制出每条线段
   Mat dst0;
   src.copyTo(dst0);
   for (size_t i = 0; i < lines.size(); i++)
   {
       float rho = lines[i][0], theta = lines[i][1];
       Point pt1, pt2;
       double a = cos(theta), b = sin(theta);
       double x0 = a*rho, y0 = b*rho;
       pt1.x = cvRound(x0 + 1000 * (-b));
       pt1.y = cvRound(y0 + 1000 * (a));
       pt2.x = cvRound(x0 - 1000 * (-b));
       pt2.y = cvRound(y0 - 1000 * (a));
       cv::line(dst0, pt1, pt2, cv::Scalar(0, 0, 255), 2, CV_AA);
   }
   imshow("霍夫变换_OpenCV", dst0);

   //自己定义的函数
   lines.clear();
   int linenum = 3;
   Mat dst1;
   src.copyTo(dst1);
   CumstomHough(gray, lines, linenum);
   for(int k = 0; k < lines.size(); k++)
   {
       for(int i = 0; i < src.rows; i++)
       {
           for(int j=0; j < src.cols; j++)
           {
                //根据theta计算rho
                double radian = lines[k][1] *  3.1415926 / 180.0;
               int dist = j * cos(radian) + i * sin(radian);
               if(dist == lines[k][0])
               {
                   dst1.at<cv::Vec3b>(i, j)[0] = 0;
                   dst1.at<cv::Vec3b>(i, j)[1] = 0;
                   dst1.at<cv::Vec3b>(i, j)[2] = 255;
               }


           }
       }
   }
   imshow("霍夫变换_Custom", dst1);

   waitKey(0);
   return 0;
}
