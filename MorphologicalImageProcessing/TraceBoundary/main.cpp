/**********************************************************************

Copyright (c) Mr.Bin. All rights reserved.
For more information visit: http://blog.csdn.net/webzhuce

**********************************************************************/
#include<opencv2/opencv.hpp>
using namespace cv;

//only process binary image
//black is boundary
std::vector<std::vector<cv::Point>> TraceBoundary(Mat &image)
{
    std::vector<std::vector<Point>> boundaryset;
    Point start, current, next; // start point and current point
    //search dirction array
    int direction[8][2] ={{-1, 1}, //left-down
                          {0, 1}, // down
                          {1, 1}, //right-down
                          {1, 0}, //right
                          {1, -1}, //right-up
                          {0, -1}, //up
                          {-1, -1}, //left-up
                          {-1, 0}  // left
                         };
    int begindirection = 0, currentdirection = 0;
    bool atstart = false, findboundary = false;

    for(int i = 0; i < image.rows; i++)
    {
        for(int j = 0; j < image.cols; j++)
        {
            if(image.at<uchar>(i, j) == 0) //find start point
            {
                start.x = j;
                start.y = i;
                current = start;
                atstart = true;
                findboundary =  true;
                std::vector<Point> points;
                points.push_back(current);
                std::cout << "Start: " << j << " " << i << std::endl;
                while((current.x != start.x) || (current.y != start.y) || atstart)
                {
                    atstart = false;

                    //search next point
                    next.x = current.x + direction[currentdirection][0];
                    next.y = current.y + direction[currentdirection][1];
                    int searchtimes = 1;
                    while(next.x < 0 || next.x >= image.cols || next.y < 0 || next.y >= image.rows || image.at<uchar>(next) == 255)
                    {
                        currentdirection++; //rotate 45 degrees counterclockwise
                        currentdirection %= 8;
                        next.x = current.x + direction[currentdirection][0];
                        next.y = current.y + direction[currentdirection][1];
                        //there are no boundary points in 8 domains, which means they are isolated points
                        if(++searchtimes >= 8)
                            break;
                    }
                    if(image.at<uchar>(next) == 0) // find next point
                    {
                        std::cout << "Next: " << next.x << " " << next.y << std::endl;
                        points.push_back(next);
                        current = next;
                        currentdirection -= 2;
                        if(currentdirection < 0)
                            currentdirection += 8;

                    }
                    else // not find next point
                    {
                        findboundary = false;
                        break;
                    }
                }
                if(findboundary)
                {
                    boundaryset.push_back(points);
                    for(auto &p : points)
                    {
                        image.at<uchar>(p) = 255;
                    }
                }
            } // find boundary one time
        } // for j
    } // for i
    return boundaryset;
}

int main(int argc, char *argv[])
{
   Mat originimage = imread("../TraceBoundary/head_boundary.bmp");
   imshow("OriginImage", originimage);

   Mat image;
   cvtColor(originimage, image, CV_BGR2GRAY);
   std::vector<std::vector<Point>> boundaryset = TraceBoundary(image);

   //show result
   Mat result;
   originimage.copyTo(result);
   for(auto &points : boundaryset)
   {
       for(auto &p : points)
       {
            result.at<Vec3b>(p)[0]= 0;
            result.at<Vec3b>(p)[0]= 0;
            result.at<Vec3b>(p)[1]= 255;
       }
   }
   imshow("Output", result);


    waitKey(0);
    return 0;

}
