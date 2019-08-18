/**********************************************************************

Copyright (c) Mr.Bin. All rights reserved.
For more information visit: http://blog.csdn.net/webzhuce

**********************************************************************/
# include<opencv2/opencv.hpp>
# include<iostream>
#include "wavelet.h"

using namespace std;
using namespace cv;

int main() {
    Mat image = imread("../Wavelet/image.png", 0);
	imshow("OriginImage", image);

    Mat decomposition = DWT2(image, 1);
	Mat uchardecomposition;
	decomposition.convertTo(uchardecomposition, CV_8UC1);
	imshow("Decomposition", uchardecomposition);
	//imwrite("result.png", ucharresult);

    Mat reconstruction = IDWT2(decomposition, 1);
	Mat ucharreconstruction;
	reconstruction.convertTo(ucharreconstruction, CV_8UC1);
	imshow("Reconstruction", ucharreconstruction);
	imwrite("reconstruction.png", ucharreconstruction);

	Mat diff = ucharreconstruction - image;
	imshow("diff", diff);
	waitKey(0);
	return 0;
}
