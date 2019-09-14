/**********************************************************************

Copyright (c) Mr.Bin. All rights reserved.
For more information visit: http://blog.csdn.net/webzhuce

**********************************************************************/
#include "snakeimage.h"

SnakeImage::SnakeImage(const cv::Mat &src)
{
	if (src.channels() == 3) 
	{
		cv::cvtColor(src, src_, CV_BGR2GRAY);
		src.copyTo(image_);
	}	
	else
	{
		src.copyTo(src_);
		cv::cvtColor(src, image_, CV_GRAY2BGR);
	}
	//set criteria for iteration termination
	termcriteria_.type = CV_TERMCRIT_ITER;
	termcriteria_.maxCount = 1000;
	termcriteria_.epsilon = 0.1;
}

void SnakeImage::segment(std::vector<cv::Point> &points)
{
	drawCurve(points);
	if (points.size() < 3)
		return;

	int i = 0, j = 0, k = 0;
	//win_ is a domain, in which  search minimization of energy for each point
	//neighbors is the number of points in win_
	int neighbors = win_.height * win_.width;
	//the center of win_
	int centerx = win_.width >> 1;
	int centery = win_.height >> 1;

	//energy terms of neighbors of every points
	float *Econt = new float[neighbors];
	float *Ecurv = new float[neighbors];
	float *Eimg = new float[neighbors];
	float *E = new float[neighbors];

	//for two order difference
	int tx = 0, ty = 0;

	//for calculating distance
	float averagedistance = 0, diffx = 0, diffy = 0;

	//for calculating moving
	int offsetx = 0, offsety = 0;

	//temporary variables
	float tmp = 0.0, energy = 0.0;

	//if SNAKE_GRAD
	cv::Mat dx, dy, src;
	src_.convertTo(src, CV_16SC1);
	Sobel(src, dx, src.depth(), 1, 0, 1);
	Sobel(src, dy, src.depth(), 0, 1, 1);

	// the number of moved points in each iteration
	int movecount = 0;

	bool converged = false;
	int iteration = 0;
	while (!converged)
	{
		movecount = 0; // the number of moved points
		iteration++;

		//the average of distance between points
		averagedistance = 0;
		for (i = 0; i < points.size(); i++)
		{
			if (i != 0)
			{
				diffx = points[i - 1].x - points[i].x;
				diffy = points[i - 1].y - points[i].y;
			}
			else //n-1->0
			{
				diffx = points[points.size() - 1].x - points[i].x;
				diffy = points[points.size() - 1].y - points[i].y;
			}
			averagedistance += std::sqrt(std::pow(diffx, 2) + std::pow(diffy, 2));
		}
		averagedistance /= points.size();

		//search each point
		for (i = 0; i < points.size(); i++)
		{
			//calculate Econt
			float maxEcont = 0;
			float minEcont = SNAKE_BIG;
			//calculate reasonable search boundaries 
			int left = std::min(points[i].x, centerx);
			int right = std::min(src_.cols - 1 - points[i].x, centerx);
			int upper = std::min(points[i].y, centery);
			int bottom = std::min(src_.rows - 1 - points[i].y, centery);
			for (j = -upper; j <= bottom; j++)
			{
				for (k = -left; k <= right; k++)
				{
					if (i == 0)
					{
						diffx = points[points.size() - 1].x - (points[i].x + k);
						diffy = points[points.size() - 1].y - (points[i].y + j);
					}
					else
					{
						diffx = points[i - 1].x - (points[i].x + k);
						diffy = points[i - 1].y - (points[i].y + j);
					}
					energy = std::abs(averagedistance - std::sqrt(std::pow(diffx, 2) + std::pow(diffy, 2)));
					Econt[(j + centery) * win_.width + k + centerx] = energy;
					maxEcont = std::max(maxEcont, energy);
					minEcont = std::min(minEcont, energy);
				}
			}
			//if maxEcont == minEcont, set Econt all to zero
			//else normalize Econt
			tmp = maxEcont - minEcont;
			tmp = (tmp == 0.0f) ? 0.0f : (1.0 / tmp);
			for (k = 0; k < neighbors; k++)
			{
				Econt[k] = (Econt[k] - minEcont) * tmp;
			}

			//calculate Ecurv
			float maxEcurv = 0.0f;
			float minEcurv = SNAKE_BIG;
			for (j = -upper; j <= bottom; j++)
			{
				for (k = -left; k <= right; k++)
				{
					if (i == 0)
					{
						tx = points[points.size() - 1].x - 2 * (points[i].x + k) + points[i + 1].x;
						ty = points[points.size() - 1].y - 2 * (points[i].y + j) + points[i + 1].y;
 					}
					else if (i == points.size() - 1)
					{
						tx = points[i - 1].x - 2 * (points[i].x + k) + points[0].x;
						ty = points[i - 1].y - 2 * (points[i].y + j) + points[0].y;
					}
					else
					{
						tx = points[i - 1].x - 2 * (points[i].x + k) + points[i + 1].x;
						ty = points[i - 1].y - 2 * (points[i].y + j) + points[i + 1].y;
					}
					energy = std::pow(tx, 2) + std::pow(ty, 2);
					Ecurv[(j + centery) * win_.width + k + centerx] = energy;
					maxEcurv = std::max(maxEcurv, energy);
					minEcurv = std::min(minEcurv, energy);
				}
			}
			// normalize as above
			tmp = maxEcurv - minEcurv;
			tmp = (tmp == 0.0f) ? 0.0f : (1.0 / tmp);
			for (k = 0; k < neighbors; k++)
			{
				Ecurv[k] = (Ecurv[k] - minEcurv) * tmp;
			}

			//calculate Eimg
			float maxEimg = 0.0f;
			float minEimg = SNAKE_BIG;
			for (j = -upper; j <= bottom; j++)
			{
				for (k = -left; k <= right; k++)
				{
					if (scheme_ == SNAKE_GRAD)
					{
						energy = std::pow(dx.at<short>(points[i].y + j, points[i].x + k), 2);
						energy += std::pow(dy.at<short>(points[i].y + j, points[i].x + k), 2);
						Eimg[(j + centery) * win_.width + k + centerx] = energy;
					}
					else
					{
						energy = src_.at<uchar>(points[i].y + j, points[i].x + k)  ;
						Eimg[(j + centery) * win_.width + k + centerx] = energy;
					}
					maxEimg = std::max(maxEimg, energy);
					minEimg = std::min(minEimg, energy);
				}
			}
			// normalize as above 
			tmp = (maxEimg - minEimg);
 			tmp = (tmp == 0.0f) ? 0.0f : (1.0 / tmp);
			for (k = 0; k < neighbors; k++)
			{
				Eimg[k] = (minEimg - Eimg[k]) * tmp;
			}

			//calculate energy of neighbors
			for (k = 0; k < neighbors; k++)
			{
				E[k] = alpha_ * Econt[k] + beta_ * Ecurv[k] + gamma_ * Eimg[k];
			}
			//find minimum energy of neighbors
			float Emin = SNAKE_BIG;
			for (j = -upper; j <= bottom; j++)
			{
				for (k = -left; k <= right; k++)
				{
					if (E[(j + centery) * win_.width + k + centerx] < Emin)
					{
						Emin = E[(j + centery) * win_.width + k + centerx];
						offsetx = k;
						offsety = j;
					}
				}
			}
			//if moved
			if (offsetx || offsety)
			{
				points[i].x += offsetx;
				points[i].y += offsety;
				movecount++;
			}
		} // for each point

		//judge convergence
		converged = (movecount == 0);
		if ((termcriteria_.type & CV_TERMCRIT_ITER) && (iteration >= termcriteria_.maxCount))
			converged = 1;
		if ((termcriteria_.type & CV_TERMCRIT_EPS) && (movecount <= termcriteria_.epsilon))
			converged = 1;

		drawCurve(points);
	} //for each iteration

	//free resource
	delete[] Econt;
	delete[] Ecurv;
	delete[] Eimg;
	delete[] E;
}

void SnakeImage::segment(std::vector<cv::Point> &points, std::vector<float> &alpha, std::vector<float> &beta, std::vector<float> &gamma)
{
	drawCurve(points);
	if (points.size() < 3)
		return;

	int i = 0, j = 0, k = 0;
	//win_ is a domain, in which  search minimization of energy for each point
	//neighbors is the number of points in win_
	int neighbors = win_.height * win_.width;
	//the center of win_
	int centerx = win_.width >> 1;
	int centery = win_.height >> 1;

	//energy terms of neighbors of every points
	float *Econt = new float[neighbors];
	float *Ecurv = new float[neighbors];
	float *Eimg = new float[neighbors];
	float *E = new float[neighbors];

	//for two order difference
	int tx = 0, ty = 0;

	//for calculating distance
	float averagedistance = 0, diffx = 0, diffy = 0;

	//for calculating moving
	int offsetx = 0, offsety = 0;

	//temporary variables
	float tmp = 0.0, energy = 0.0;

	//if SNAKE_GRAD
	cv::Mat dx, dy, src;
	src_.convertTo(src, CV_16SC1);
	Sobel(src, dx, src.depth(), 1, 0, 1);
	Sobel(src, dy, src.depth(), 0, 1, 1);

	// the number of moved points in each iteration
	int movecount = 0;

	bool converged = false;
	int iteration = 0;
	while (!converged)
	{
		movecount = 0; // the number of moved points
		iteration++;

		//the average of distance between points
		averagedistance = 0;
		for (i = 0; i < points.size(); i++)
		{
			if (i != 0)
			{
				diffx = points[i - 1].x - points[i].x;
				diffy = points[i - 1].y - points[i].y;
			}
			else //n-1->0
			{
				diffx = points[points.size() - 1].x - points[i].x;
				diffy = points[points.size() - 1].y - points[i].y;
			}
			averagedistance += std::sqrt(std::pow(diffx, 2) + std::pow(diffy, 2));
		}
		averagedistance /= points.size();

		//search each point
		for (i = 0; i < points.size(); i++)
		{
			//calculate Econt
			float maxEcont = 0;
			float minEcont = SNAKE_BIG;
			//calculate reasonable search boundaries 
			int left = std::min(points[i].x, centerx);
			int right = std::min(src_.cols - 1 - points[i].x, centerx);
			int upper = std::min(points[i].y, centery);
			int bottom = std::min(src_.rows - 1 - points[i].y, centery);
			for (j = -upper; j <= bottom; j++)
			{
				for (k = -left; k <= right; k++)
				{
					if (i == 0)
					{
						diffx = points[points.size() - 1].x - (points[i].x + k);
						diffy = points[points.size() - 1].y - (points[i].y + j);
					}
					else
					{
						diffx = points[i - 1].x - (points[i].x + k);
						diffy = points[i - 1].y - (points[i].y + j);
					}
					energy = std::abs(averagedistance - std::sqrt(std::pow(diffx, 2) + std::pow(diffy, 2)));
					Econt[(j + centery) * win_.width + k + centerx] = energy;
					maxEcont = std::max(maxEcont, energy);
					minEcont = std::min(minEcont, energy);
				}
			}
			//if maxEcont == minEcont, set Econt all to zero
			//else normalize Econt
			tmp = maxEcont - minEcont;
			tmp = (tmp == 0.0f) ? 0.0f : (1.0 / tmp);
			for (k = 0; k < neighbors; k++)
			{
				Econt[k] = (Econt[k] - minEcont) * tmp;
			}

			//calculate Ecurv
			float maxEcurv = 0.0f;
			float minEcurv = SNAKE_BIG;
			for (j = -upper; j <= bottom; j++)
			{
				for (k = -left; k <= right; k++)
				{
					if (i == 0)
					{
						tx = points[points.size() - 1].x - 2 * (points[i].x + k) + points[i + 1].x;
						ty = points[points.size() - 1].y - 2 * (points[i].y + j) + points[i + 1].y;
					}
					else if (i == points.size() - 1)
					{
						tx = points[i - 1].x - 2 * (points[i].x + k) + points[0].x;
						ty = points[i - 1].y - 2 * (points[i].y + j) + points[0].y;
					}
					else
					{
						tx = points[i - 1].x - 2 * (points[i].x + k) + points[i + 1].x;
						ty = points[i - 1].y - 2 * (points[i].y + j) + points[i + 1].y;
					}
					energy = std::pow(tx, 2) + std::pow(ty, 2);
					Ecurv[(j + centery) * win_.width + k + centerx] = energy;
					maxEcurv = std::max(maxEcurv, energy);
					minEcurv = std::min(minEcurv, energy);
				}
			}
			// normalize as above
			tmp = maxEcurv - minEcurv;
			tmp = (tmp == 0.0f) ? 0.0f : (1.0 / tmp);
			for (k = 0; k < neighbors; k++)
			{
				Ecurv[k] = (Ecurv[k] - minEcurv) * tmp;
			}

			//calculate Eimg
			float maxEimg = 0.0f;
			float minEimg = SNAKE_BIG;
			for (j = -upper; j <= bottom; j++)
			{
				for (k = -left; k <= right; k++)
				{
					if (scheme_ == SNAKE_GRAD)
					{
						energy = std::pow(dx.at<short>(points[i].y + j, points[i].x + k), 2);
						energy += std::pow(dy.at<short>(points[i].y + j, points[i].x + k), 2);
						Eimg[(j + centery) * win_.width + k + centerx] = energy;
					}
					else
					{
						energy = src_.at<uchar>(points[i].y + j, points[i].x + k);
						Eimg[(j + centery) * win_.width + k + centerx] = energy;
					}
					maxEimg = std::max(maxEimg, energy);
					minEimg = std::min(minEimg, energy);
				}
			}
			// normalize as above 
			tmp = (maxEimg - minEimg);
			tmp = (tmp == 0.0f) ? 0.0f : (1.0 / tmp);
			for (k = 0; k < neighbors; k++)
			{
				Eimg[k] = (minEimg - Eimg[k]) * tmp;
			}

			//calculate energy of neighbors
			for (k = 0; k < neighbors; k++)
			{
				E[k] = alpha[i] * Econt[k] + beta[i] * Ecurv[k] + gamma[i] * Eimg[k];
			}
			//find minimum energy of neighbors
			float Emin = SNAKE_BIG;
			for (j = -upper; j <= bottom; j++)
			{
				for (k = -left; k <= right; k++)
				{
					if (E[(j + centery) * win_.width + k + centerx] < Emin)
					{
						Emin = E[(j + centery) * win_.width + k + centerx];
						offsetx = k;
						offsety = j;
					}
				}
			}
			//if moved
			if (offsetx || offsety)
			{
				points[i].x += offsetx;
				points[i].y += offsety;
				movecount++;
			}
		} // for each point

		  //judge convergence
		converged = (movecount == 0);
		if ((termcriteria_.type & CV_TERMCRIT_ITER) && (iteration >= termcriteria_.maxCount))
			converged = 1;
		if ((termcriteria_.type & CV_TERMCRIT_EPS) && (movecount <= termcriteria_.epsilon))
			converged = 1;

		drawCurve(points);
	} //for each iteration

	  //free resource
	delete[] Econt;
	delete[] Ecurv;
	delete[] Eimg;
	delete[] E;

}

void SnakeImage::drawCurve(std::vector<cv::Point> &points)
{
	cv::Mat image = image_.clone();
	for (int i = 0; i < points.size(); i++)
	{
		int j = (i + 1) % points.size();
		cv::line(image, points[i], points[j], CV_RGB(255, 0, 0), 3, 8, 0);
	}
	cv::imshow("Iteration", image);
	cv::waitKey(0);
}
