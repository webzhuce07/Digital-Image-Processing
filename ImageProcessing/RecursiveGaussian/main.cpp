/**********************************************************************

Copyright (c) Mr.Bin. All rights reserved.
For more information visit: http://blog.csdn.net/webzhuce

**********************************************************************/
#include <opencv2/opencv.hpp>
using namespace cv;

typedef struct
{
    int    N;
    float  sigma;
    double B;
    double b[4];
} GaussCoefs;

/*
* Calculate the coefficients for the recursive filter algorithm
* Fast Computation of gaussian blurring.
*/
void ComputeGaussCoefs(GaussCoefs *c, float sigma)
{
    /*
    * Papers:  "Recursive Implementation of the gaussian filter.",
    *          Ian T. Young , Lucas J. Van Vliet, Signal Processing 44, Elsevier 1995.
    * formula: 11b       computation of q
    *          8c        computation of b0..b1
    *          10        alpha is normalization constant B
    */
    float q = 0, q2 = 0, q3 = 0;
    if (sigma >= 2.5)
    {
        q = 0.98711 * sigma - 0.96330;
    }
    else if ((sigma >= 0.5) && (sigma < 2.5))
    {
        q = 3.97156 - 4.14554 * (float)sqrt((double)1 - 0.26891 * sigma);
    }
    else
    {
        q = 0.1147705018520355224609375;
    }

    q2 = q * q;
    q3 = q * q2;
    c->b[0] = (1.57825 + (2.44413*q) + (1.4281 *q2) + (0.422205*q3));
    c->b[1] = ((2.44413*q) + (2.85619*q2) + (1.26661 *q3));
    c->b[2] = (-((1.4281*q2) + (1.26661 *q3)));
    c->b[3] = ((0.422205*q3));
    c->B = 1.0 - ((c->b[1] + c->b[2] + c->b[3]) / c->b[0]);
    c->sigma = sigma;
    c->N = 3;
}

void RecursiveGausssmooth(float *in, float *out, int size, int rowstride, GaussCoefs *c)
{
    /*
    * Papers:  "Recursive Implementation of the gaussian filter.",
    *          Ian T. Young , Lucas J. Van Vliet, Signal Processing 44, Elsevier 1995.
    * formula: 9a        forward filter
    *          9b        backward filter
    *          fig7      algorithm
    */
    int i, n, bufsize;
    float *w1, *w2;

    /* forward pass */
    bufsize = size + 3;
    size -= 1;
    w1 = new float[bufsize];
    w2 = new float[bufsize];
    w1[0] = in[0];
    w1[1] = in[0];
    w1[2] = in[0];
    for (i = 0, n = 3; i <= size; i++, n++)
    {
        w1[n] = (float)(c->B*in[i*rowstride] +
            ((c->b[1] * w1[n - 1] +
                c->b[2] * w1[n - 2] +
                c->b[3] * w1[n - 3]) / c->b[0]));
    }

    /* backward pass */
    w2[size + 1] = w1[size + 3];
    w2[size + 2] = w1[size + 3];
    w2[size + 3] = w1[size + 3];
    for (i = size, n = i; i >= 0; i--, n--)
    {
        w2[n] = out[i * rowstride] = (float)(c->B*w1[n] +
            ((c->b[1] * w2[n + 1] +
                c->b[2] * w2[n + 2] +
                c->b[3] * w2[n + 3]) / c->b[0]));
    }

    delete[] w1;
    delete[] w2;
}

/*
@function: 2D gauss filter
*/
void RecursiveGausssmooth2D(const Mat &src, Mat &dst, float sigma = 1.0f)
{
    GaussCoefs c;
    ComputeGaussCoefs(&c, sigma);
    const int height = src.rows;
    const int width = src.cols;
    if (src.channels() == 1)
    {
        Mat srccopy;
        src.convertTo(srccopy, CV_32FC1);
        Mat dstcopy;
        src.convertTo(dstcopy, CV_32FC1);
        float* srcdata = srccopy.ptr<float>(0);
        float* dstdata = dstcopy.ptr<float>(0);
        for (int y = 0; y < height; y++)
        {
            RecursiveGausssmooth(srcdata + y * width, dstdata + y * width, width, 1, &c);
        }
        for (int x = 0; x < width; x++)
        {
            RecursiveGausssmooth(dstdata + x, dstdata + x, height, width, &c);
        }
        dstcopy.convertTo(dst, src.type());
    }
    else if (src.channels() == 3)
    {
        Mat srccopy;
        src.convertTo(srccopy, CV_32FC3);
        Mat dstcopy;
        src.convertTo(dstcopy, CV_32FC3);
        for (int y = 0; y < height; y++)
        {
			float* srcrowdata = srccopy.ptr<float>(y);
			float* dstrowdata = dstcopy.ptr<float>(y);
            RecursiveGausssmooth(srcrowdata, dstrowdata, width, 3, &c);
            RecursiveGausssmooth(srcrowdata + 1, dstrowdata + 1, width, 3, &c);
            RecursiveGausssmooth(srcrowdata + 2, dstrowdata + 2, width, 3, &c);
        }
		float* srcdata = srccopy.ptr<float>(0);
		float* dstdata = dstcopy.ptr<float>(0);
		for (int x = 0; x < width; x++)
		{
			RecursiveGausssmooth(dstdata + x * 3, dstdata + x * 3, height, width * 3, &c);
			RecursiveGausssmooth(dstdata + x * 3 + 1, dstdata + x * 3 + 1, height, width * 3, &c);
			RecursiveGausssmooth(dstdata + x * 3 + 2, dstdata + x * 3 + 2, height, width * 3, &c);
		}
        dstcopy.convertTo(dst, src.type());
    }
    else
        return;
}

int main(int argc, char *argv[])
{
    Mat src = imread("../RecursiveGaussian/lena.bmp", 1);
    imshow("original", src);

    Mat dst;
    RecursiveGausssmooth2D(src, dst);
    imshow(" RecursiveGaussSmooth", dst);

	Mat dstImage;
	GaussianBlur(src, dstImage, Size(5, 5), 2, 2);
	imshow("GaussSmooth", dstImage);

    waitKey(0);
    return EXIT_SUCCESS;
}
