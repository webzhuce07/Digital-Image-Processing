#ifndef __splitandmerge_h__
#define __splitandmerge_h__

#include <opencv2/opencv.hpp>
#include <vector>

struct Quadtrees {
    // tree data structure
    std::vector<Quadtrees> childs;
    bool validity;     //if unmerged, validity = true
    cv::Rect roi;     //region of interest
    Quadtrees(cv::Rect rect, bool flag)
       :roi(rect),
        validity(flag)
    {
    }
    Quadtrees() = default;
};

class SplitAndMerge
{
public:
    cv::Mat excute();
    SplitAndMerge(cv::Mat image);
    ~SplitAndMerge();

private:
    bool predicateForSplit(const Quadtrees& r);
    bool predicateForMerge(const Quadtrees& r1, const Quadtrees& r2);
    void merge(Quadtrees& r);
    void getRegion(Quadtrees& r, std::vector<Quadtrees*>& regions);
    bool isNeighbour(const Quadtrees& r1, const Quadtrees& r2);
    void mergeNeighbours(Quadtrees& r1, Quadtrees& r2);
    Quadtrees split(const cv::Rect& roi);
    cv::Mat source_image_;
    cv::Mat gray_image_;
    cv::Mat result_image_;
    int threshold_ = 3;
};
#endif // __splitandmerge_h__
