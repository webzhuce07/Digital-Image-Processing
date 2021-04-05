#include <iostream>
#include <opencv2/opencv.hpp>

cv::Mat regionGrow(const cv::Mat& src, const cv::Point2i seed, int throld);
cv::Mat regionGrowFast(const cv::Mat& src, const cv::Point2i seed, int throld);
void onMouse(int event, int x, int y, int flags, void* param) {

	cv::Mat *image = reinterpret_cast<cv::Mat*>(param);

	switch (event) 
	{	// dispatch the event

	case cv::EVENT_LBUTTONDOWN: // 鼠标左键按下
		cv::Point2i seed_point(x, y);
		cv::Mat show_image = image->clone();
		cv::circle(show_image, seed_point, 1, cv::Scalar(0, 0, 255), 1);
		cv::namedWindow("ShowPoint");
		cv::imshow("ShowPoint", show_image);
		int throld = 4;
		cv::Mat result = regionGrow(*image, seed_point, throld);
		cv::namedWindow("regionGrow");
		cv::imshow("regionGrow", result);
		cv::Mat result_fast = regionGrowFast(*image, seed_point, throld);
		cv::namedWindow("regionGrowFast");
		cv::imshow("regionGrowFast", result_fast);

		break;
	}
}

int main()
{
	cv::Mat image = cv::imread("D:\\TestData\\VTK-logo.png");
	cv::namedWindow("Original Image"); 
	cv::imshow("Original Image", image); 
	// set the mouse callback for this image
	cv::setMouseCallback("Original Image", onMouse, reinterpret_cast<void*>(&image));

	cv::waitKey(0); 
	return 0;

}

//参考《数字图像处理与机器学习》
cv::Mat regionGrow(const cv::Mat &src, const cv::Point2i seed, int throld)
{
	//convert src to gray for getting gray value of every pixel
	cv::Mat gray;
	cv::cvtColor(src, gray, cv::COLOR_RGB2GRAY);

	// set every pixel to black
	cv::Mat result = cv::Mat::zeros(src.size(), CV_8UC1);
	if ((seed.x < 0) || (seed.y < 0))
		return result;
	result.at<uchar>(seed.y, seed.x) = 255;

	//count: sum of points that meet the conditions in every eight-neighbor
	int count = 1;
	//start growing
	while (count > 0) {
		count = 0;
		for (int row = 1; row < gray.rows - 1; row++) {
			for (int col = 1; col < gray.cols - 1; col++) {
				if (result.at<uchar>(row, col) != 255)
					continue;
				//gray value of current seed
				int seed_gray = gray.at<uchar>(row, col);
				for (int m = row - 1; m <= row + 1; m++) {
					for (int n = col - 1; n <= col + 1; n++) {
						int value = gray.at<uchar>(m, n);
						if ((result.at<uchar>(m, n) == 0) && (abs(value - seed_gray) <= throld)) {
							result.at<uchar>(m, n) = 255;
							count++;
						}
					}
				}
			}
		}
	}
	return result;
}

cv::Mat regionGrowFast(const cv::Mat &src, const cv::Point2i seed, int throld)
{
	//convert src to gray for getting gray value of every pixel
	cv::Mat gray;
	cv::cvtColor(src, gray, cv::COLOR_RGB2GRAY);

	// set every pixel to black
	cv::Mat result = cv::Mat::zeros(src.size(), CV_8UC1);
	if ((seed.x < 0) || (seed.y < 0))
		return result;
	result.at<uchar>(seed.y, seed.x) = 255;

	//grow direction sequenc
	int grow_direction[8][2] = { {-1,-1}, {0,-1}, {1,-1}, {1,0}, {1,1}, {0,1}, {-1,1}, {-1,0} };
	//seeds collection
	std::vector<cv::Point2i> seeds;
	seeds.push_back(seed);

	//start growing
	while (!seeds.empty()) {
		//get a seed
		cv::Point2i current_seed = seeds.back();
		seeds.pop_back();
		//gray value of current seed
		int seed_gray = gray.at<uchar>(current_seed.y, current_seed.x);

		for (int i = 0; i < 8; ++i) {
			cv::Point2i neighbor_seed(current_seed.x + grow_direction[i][0], current_seed.y + grow_direction[i][1]);
			//check wether in image
			if (neighbor_seed.x < 0 || neighbor_seed.y < 0 || neighbor_seed.x >(gray.cols - 1) || (neighbor_seed.y > gray.rows - 1))
				continue;
			int value = gray.at<uchar>(neighbor_seed.y, neighbor_seed.x);
			if ((result.at<uchar>(neighbor_seed.y, neighbor_seed.x) == 0) && (abs(value - seed_gray) <= throld)) {
				result.at<uchar>(neighbor_seed.y, neighbor_seed.x) = 255;
				seeds.push_back(neighbor_seed);
			}
		}
	}
	return result;
}