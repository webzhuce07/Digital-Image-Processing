#include "splitandmerge.h"

using namespace std;
using namespace cv;

SplitAndMerge::SplitAndMerge(Mat image)
    :source_image_(image)
{
    //Make sure that the image width and height are 2 integer powers
    int exponent = log(min(source_image_.cols, source_image_.rows)) / log(2);
    int s = pow(2.0, (double)exponent);
    Rect square = Rect(0, 0, s, s);
    Mat img = source_image_(square).clone();
    //generate gray image
    cvtColor(img, gray_image_, CV_BGR2GRAY);
    result_image_ = Mat(gray_image_.size(),CV_8UC1,  Scalar(0));
    namedWindow("gray", CV_WINDOW_AUTOSIZE);
    imshow("gray", gray_image_);
}

SplitAndMerge::~SplitAndMerge()
{
}

Mat SplitAndMerge::excute()
{
    Quadtrees r = split(Rect(0, 0, gray_image_.cols, gray_image_.rows));
    merge(r);
    namedWindow("result", CV_WINDOW_AUTOSIZE);
    imshow("result", result_image_);
    return result_image_;
}

/***************************************************************
 * 注释部分原理类似Matlab的qtdecomp、qtgetblk和gtsetblk：如果字块中最大
 * 像素和最小像素的差值小于阈值threshold_就认为满足一致性条件
***************************************************************/
bool SplitAndMerge::predicateForSplit(const Quadtrees& r)
{
//    uchar  max = 0, min = 255;
//    for(int i = 0; i < r.roi.width; ++i){
//        for(int j = 0; j < r.roi.height; ++j){
//            uchar value = gray_image_.at<uchar>(j + r.roi.y, i + r.roi.x);
//            if(value > max)
//                max = value;
//            if(value < min)
//                min = value;
//        }
//    }
//    return ((max - min) < threshold_);
    Scalar stddev, mean;
    meanStdDev(gray_image_(r.roi), mean, stddev);
    return (abs(mean[0] - 76) < threshold_);
}

bool SplitAndMerge::predicateForMerge(const Quadtrees& r1, const Quadtrees& r2)
{
//    uchar  max = 0, min = 255;
//    for(int i = 0; i < r1.roi.width; ++i){
//        for(int j = 0; j < r1.roi.height; ++j){
//            uchar value = gray_image_.at<uchar>(j + r1.roi.y, i + r1.roi.x);
//            if(value > max)
//                max = value;
//            if(value < min)
//                min = value;
//        }
//    }
//    for(int i = 0; i < r2.roi.width; ++i){
//        for(int j = 0; j < r2.roi.height; ++j){
//            uchar value = gray_image_.at<uchar>(j + r2.roi.y, i + r2.roi.x);
//            if(value > max)
//                max = value;
//            if(value < min)
//                min = value;
//        }
//    }
//    return ((max - min) < threshold_);
    Scalar stddev1, mean1;
    meanStdDev(gray_image_(r1.roi), mean1, stddev1);
    Scalar stddev2, mean2;
    meanStdDev(gray_image_(r2.roi), mean2, stddev2);
    return (abs(mean1[0] - 76) < threshold_) && (abs(mean2[0] - 76) < threshold_);
}

void SplitAndMerge::mergeNeighbours(Quadtrees &r1, Quadtrees &r2)
{
    if(predicateForMerge(r1, r2)){
        for(int i = 0; i < r1.roi.width; ++i){
            for(int j = 0; j < r1.roi.height; ++j)
                result_image_.at<uchar>(j + r1.roi.y, i + r1.roi.x) = 255;
        }
        for(int i = 0; i < r2.roi.width; ++i){
            for(int j = 0; j < r2.roi.height; ++j)
                result_image_.at<uchar>(j + r2.roi.y, i + r2.roi.x) = 255;
        }
    }
}
/***********************************************************************
 *难点1：哪些字块需要合并？我认为是子块没有childs的，函数getRegion就是获取需要合并
 *      的子块的集合
 *难点2：检查是否是相邻区域，函数isNeighbour就是要来判断的。
 *函数mergeNeighbours就是合并具有相似特征的区域，合并后result_iamge_颜色设为白色
 * 合并还需提高运行速度，如果有更好的方法，方便地告诉我。谢谢。
***********************************************************************/
void SplitAndMerge::merge(Quadtrees& r)
{
    if(r.childs.size() == 0){
        for(int i = 0; i < r.roi.width; ++i)
            for(int j = 0; j < r.roi.height; ++j)
                gray_image_.at<uchar>(j + r.roi.y, i + r.roi.x) = 255;
       return;
    }
    vector<Quadtrees*> all_region;
    getRegion(r, all_region);
    for(vector<Quadtrees*>::iterator t1 = all_region.begin(); t1 != all_region.end(); ++t1){
        for(vector<Quadtrees*>::iterator t2 = t1 + 1; t2 != all_region.end(); ++t2){
            if(isNeighbour(*(*t1), *(*t2)))
                mergeNeighbours(*(*t1), *(*t2));
        }
   }
}

void SplitAndMerge::getRegion(Quadtrees &r, std::vector<Quadtrees *> &regions)
{
    if(r.childs.size() == 0)
        regions.push_back(&r);
    else{
      for(int i = 0; i < 4; ++i)
        getRegion(r.childs[i], regions);
    }
}

bool SplitAndMerge::isNeighbour(const Quadtrees &r1, const Quadtrees &r2)
{
    Point corners[4];
    Point increment[4][2] = {{Point(-1, 0), Point(0, -1)},
                             {Point(1, 0), Point(0, -1)},
                             {Point(1, 0), Point(0, 1)},
                             {Point(-1, 0), Point(0, 1)} };
    corners[0] = Point(r1.roi.x, r1.roi.y);
    corners[1] = Point(r1.roi.x + r1.roi.width, r1.roi.y);
    corners[2] = Point(r1.roi.x + r1.roi.width, r1.roi.y + r1.roi.height);
    corners[3] = Point(r1.roi.x, r1.roi.y + r1.roi.height);
    for(int i = 0; i < 4; ++i){
        if(!r2.roi.contains(corners[i]))
            continue;
        if(r2.roi.contains(corners[i] + increment[i][0]) && r2.roi.contains(corners[i] + increment[i][1]))  //排除对角
            return false;
        return true;
    }
    return false;
}

/**********************************************************
 *quadtree splitting: 4 意味着子块最小得4*4，提高运行速度
************************************************************/
Quadtrees SplitAndMerge::split(const Rect& roi)
{
    Quadtrees r(roi, true);
    if(roi.width > 4 &&(!predicateForSplit(r))) {
        int w = roi.width / 2;
        int h = roi.height / 2;
        Quadtrees r1 = split(Rect(roi.x, roi.y, w, h));
        Quadtrees r2 = split(Rect(roi.x + w, roi.y, w, h));
        Quadtrees r3 = split(Rect(roi.x, roi.y + h, w, h));
        Quadtrees r4 = split(Rect(roi.x + w, roi.y + h, w, h));
        r.childs.push_back(r1);
        r.childs.push_back(r2);
        r.childs.push_back(r3);
        r.childs.push_back(r4);
    }
    return r;
}
