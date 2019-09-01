/**********************************************************************

Copyright (c) Mr.Bin. All rights reserved.
For more information visit: http://blog.csdn.net/webzhuce

**********************************************************************/
#include<opencv2/opencv.hpp>
using namespace cv;

/*
 *only process binary image
 * white is foreground
*/
void Thining(const cv::Mat &src, cv::Mat &dst)
{

    //four conditions
    bool condition1 = false, condition2 = false, condition3 = false, condition4 = false;

    //n*n neighbours
    constexpr int N = 5;
    uchar neighbour[N][N];
    const int kOffset = N / 2;
    src.copyTo(dst);

    bool modified = true;
    while(modified)
    {
        modified = false;
        Mat img;
        dst.copyTo(img);

        for(int i = kOffset; i < (dst.rows - kOffset); i++)
        {
            for(int j = kOffset; j < (dst.cols - kOffset); j++)
            {
                if(dst.at<uchar>(i, j) == 0)
                    continue;

                condition1 = false;
                condition2 = false;
                condition3 = false;
                condition4 = false;

                //get N*N neighbours of current point
                //white : 0, black : 1
                for(int m = 0; m < N; m++)
                {
                    for(int n = 0; n < N; n++)
                    {
                        neighbour[m][n] = dst.at<uchar>(i + n - kOffset, j + m - kOffset) == 255 ? 1 : 0;
                    }
                }

                //2 <= NZ(P1) <=6
                int count = neighbour[1][1] + neighbour[1][2] + neighbour[1][3] +
                        neighbour[2][1] + neighbour[2][3] +
                        neighbour[3][1] + neighbour[3][2] + neighbour[3][3];
                if(count >= 2 && count <=6)
                    condition1 = true;

                //Z0(P1) == 1
                count = 0;
                if(neighbour[1][2] == 0 && neighbour[1][1] == 1)
                    count++;
                if(neighbour[1][1] == 0 && neighbour[2][1] == 1)
                    count++;
                if(neighbour[2][1] == 0 && neighbour[3][1] == 1)
                    count++;
                if(neighbour[3][1] == 0 && neighbour[3][2] == 1)
                    count++;
                if(neighbour[3][2] == 0 && neighbour[3][3] == 1)
                    count++;
                if(neighbour[3][3] == 0 && neighbour[2][3] == 1)
                    count++;
                if(neighbour[2][3] == 0 && neighbour[1][3] == 1)
                    count++;
                if(neighbour[1][3] == 0 && neighbour[1][2] == 1)
                    count++;
                if(count == 1)
                    condition2 = true;

                //P2*P4*P8 = 0 or Z0(P2) != 1
                if(neighbour[1][2] * neighbour[2][1] * neighbour[2][3] == 0)
                    condition3 = true;
                else
                {
                    count = 0;
                    if(neighbour[0][2] == 0 && neighbour[0][1] == 1)
                        count++;
                    if(neighbour[0][1] == 0 && neighbour[1][1] == 1)
                        count++;
                    if(neighbour[1][1] == 0 && neighbour[2][1] == 1)
                        count++;
                    if(neighbour[2][1] == 0 && neighbour[2][2] == 1)
                        count++;
                    if(neighbour[2][2] == 0 && neighbour[2][3] == 1)
                        count++;
                    if(neighbour[2][3] == 0 && neighbour[1][3] == 1)
                        count++;
                    if(neighbour[1][3] == 0 && neighbour[0][3] == 1)
                        count++;
                    if(neighbour[0][3] == 0 && neighbour[0][2] == 1)
                        count++;
                    if(count != 1)
                        condition3 = true;
                }

                //P2*P4*P6 = 0 or Z0(P4) != 1
                if(neighbour[1][2] * neighbour[2][1] * neighbour[3][2] == 0)
                    condition4 = true;
                else
                {
                    count = 0;
                    if(neighbour[1][1] == 0 && neighbour[1][0] == 1)
                        count++;
                    if(neighbour[1][0] == 0 && neighbour[2][0] == 1)
                        count++;
                    if(neighbour[2][0] == 0 && neighbour[3][0] == 1)
                        count++;
                    if(neighbour[3][0] == 0 && neighbour[3][1] == 1)
                        count++;
                    if(neighbour[3][1] == 0 && neighbour[3][2] == 1)
                        count++;
                    if(neighbour[3][2] == 0 && neighbour[2][2] == 1)
                        count++;
                    if(neighbour[2][2] == 0 && neighbour[1][2] == 1)
                        count++;
                    if(neighbour[1][2] == 0 && neighbour[1][1] == 1)
                        count++;
                    if(count != 1)
                        condition4 = true;
                }

                if(condition1 && condition2 && condition3 && condition4)
                {
                    img.at<uchar>(i, j) = 0;
                    modified = true;
                }
                else
                    img.at<uchar>(i, j) = 255;

            } // for columns
        } // for rows
        img.copyTo(dst);
    } //  for while
}

int main(int argc, char *argv[])
{
   Mat originimage = imread("../Thining/thining_VC.bmp", 0);
   imshow("OriginImage", originimage);
   //white is foreground
   Mat image;
   bitwise_not(originimage, image);
   imshow("Image", image);

   Mat output;
   Thining(image, output);
   bitwise_not(output, output);
   imshow("Output", output);

    waitKey(0);
    return 0;

}
