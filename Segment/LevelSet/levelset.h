/**********************************************************************

Copyright (c) Mr.Bin. All rights reserved.
For more information visit: http://blog.csdn.net/webzhuce

**********************************************************************/
/**
* @brief   This is a filter that implements  Tony F. Chan. Active 
*          Contours Without Edges[J].2001
*/
#pragma once
#include "opencv2/opencv.hpp"

class LevelSet
{
public:
	LevelSet(const cv::Mat &src);
	//initialize level set
	void initializePhi(cv::Point2f center, float radius);
	void evolving();	

private:
	int iterationnum_ = 100;
	float lambda1_ = 1.0f;	 //weight for c1 fitting term
	float lambda2_ = 1.0f;   //weight for c2 fitting term
	float mu_ = 0.1 * 255 * 255;	//¦Ì: weight for length term
	float nu_ = 0.0;  //¦Í: weight for area term, default value 0 
	float timestep_ = 0.1; //time step: ¦Ät
	//¦Å: parameter for computing smooth Heaviside and dirac function
	float epsilon_ = 1.0;	
	float c1_;	//average(u0) inside level set
	float c2_;	//average(u0) outside level set
	cv::Mat phi_;		//level set: ¦Õ
	cv::Mat dirac_;		//dirac level set: ¦Ä(¦Õ)
	cv::Mat heaviside_;	//heaviside£º§¯(¦Õ)
	cv::Mat curv_;		
	cv::Mat src_;
	cv::Mat image_; //for showing
	
	void gradient(const cv::Mat &src, cv::Mat &gradx, cv::Mat &grady);
	//Dirac function
	void dirac();
	//Heaviside function
	void heaviside();
	void calculateCurvature();
	//calculate c1 and c2
	void calculatC();
	//show evolving
	void showEvolving();
};

