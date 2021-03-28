#include <opencv2\opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
    //if (argc != 2)
    //{
    //    cout << "Usage: opencv_test <image path>" << endl;
    //    return -1;
    //}

    //char *imgName = argv[1]; 
	char *imgName = "C:\\Users\\VINNO\\Desktop\\src0\\die.jpg";
    Mat inputImg;

    inputImg = imread(imgName, 1);
    if (!inputImg.data)
    {
        cout << "No image data" << endl;
        return -1;
    }
    Mat resultImg;

	fastNlMeansDenoisingColored(inputImg, resultImg, 10, 10, 7, 21);
    imwrite("./result.jpg", resultImg);
	imshow("", resultImg);
	waitKey(0);

    return 0;
}