// Necessary headers
#include <math.h>
#include <stdlib.h>
#include <string>
#include <iostream>

#include <opencv2/opencv.hpp>
#include "radon_transform.hpp"

using namespace radon;
using namespace cv;
using namespace std;

static void help ( const char **argv ) {
  std::cout << std::endl                                                  \
            << "This program demonstrates the usage of Radon transform."  \
            << std::endl << std::endl << "USAGE: " << std::endl           \
            << argv[0] << " <filename> <angle-range-option> <operation>"  \
            << std::endl << std::endl;

  std::cout << "Default for <angle-range-option> computes for "           \
            << "all angles between 1 and 180 degrees."                    \
            << " ( @see cv::ximgproc::RadonAngleRange )" << std::endl     \
            << "Default for <operation> is RT_SUM."                       \
            << " ( @see cv::ximgproc::RadonOp )" << std::endl             \
            << std::endl;                                                 \

}

static bool argParser( int argc, const char **argv,
                       cv::Mat & img,
                       int & radonAngleRange,
                       int & radonOperation ) {
    if (argc > 4) {
        std::cout << "Incorrect arguments" << std::endl;
        return false;
    }

    const char *filename = argc >= 2 ? argv[1]
                                     : "./test.bmp";
    img = imread(filename, 0);
    if( img.empty() ) {
        std::cout << "Unable to load image: " << filename << std::endl;
        return false;
    }

    radonAngleRange = ( argc >= 3 ) ? atoi(argv[2]) : 63;     // 1 to 180 degrees
    radonOperation = ( argc >= 4 ) ? atoi(argv[3]) : RT_SUM;  // Sum up elements

    return true;
}

int main( int argc, const char ** argv ) {

  cv::Mat img;
  int radonAngleRange, radonOperation;

  // Display help
  help( argv );

  if( !argParser( argc, argv, img, radonAngleRange, radonOperation) ) {
    return -1;
  }

  cv::Mat radonTr, operImg;

  // Computing the Radon transform with appropriate params
  radonTransform( img, radonTr, radonAngleRange, radonOperation, operImg );

  // Mat for displaying the Radon transform
  cv::Mat radonTrDisp;

  double minVal, maxVal;
  minMaxLoc(radonTr, &minVal, &maxVal);

  // Normalizing radonTr so as to display as a CV_8U image
  radonTr -= minVal;
  radonTr.convertTo( radonTrDisp, CV_8U, 255.0/(maxVal-minVal) );

  // Normalizing operImg so as to display as a CV_8U image
  minMaxLoc(operImg, &minVal, &maxVal);
  operImg -= minVal;
  operImg.convertTo(operImg, CV_8U, 255.0/(maxVal-minVal));

  // Displaying the images
  cv::imshow("Input", img);
  cv::imshow("Radon transform", radonTrDisp);
  cv::imshow("Operation(image)", operImg);
  cv::imwrite("operImg.png", operImg);

  cv::waitKey(0);
  cv::destroyAllWindows();

  return 0;
}
