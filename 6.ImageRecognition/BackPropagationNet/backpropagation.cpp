#include "backpropagation.h"
#include <cmath>
#include <iostream>

double getRandomWieght()  //-1.0 ~ 1.0
{
	return ((2.0*(double)rand() / RAND_MAX) - 1);
}

//sigmoid function
double sigmoid(double x)
{
	return 1.0 / (1.0 + exp(-x));
}

BackPropagation::BackPropagation(int input_num /*= 2*/, int hide1_num /*= 3*/, int hide2_num /*= 2*/, int output_num /*= 1*/)
	:input_num_(input_num),
	hide1_num_(hide1_num),
	hide2_num_(hide2_num),
	output_num_(output_num)
{
	input_ = new double[input_num_ + 1];
	hide1_weight_ = new double*[hide1_num_];
	for(int i = 0; i < hide1_num_; ++i)
		hide1_weight_[i] = new double[input_num_ + 1];
	hide1_error_ = new double[hide1_num_];
	hide1_output_ = new double[hide1_num_ + 1];

	hide2_weight_ = new double*[hide2_num_];
	for (int i = 0; i < hide2_num_; ++i)
		hide2_weight_[i] = new double[hide1_num_ + 1];
	hide2_error_ = new double[hide2_num_];
	hide2_output_ = new double[hide2_num_ + 1];

	output_weight_ = new double*[output_num_];
	for (int i = 0; i < output_num_; ++i)
		output_weight_[i] = new double[hide2_num_ + 1];
	output_error_ = new double[output_num_];
	output_ = new double[output_num_];
	initWeight();
}

BackPropagation::~BackPropagation()
{
	delete[] input_;
	for (int i = 0; i < hide1_num_; ++i)
		delete[] hide1_weight_[i];
	delete[] hide1_weight_;	
	delete[]  hide1_error_;
	delete[] hide1_output_;

	for (int i = 0; i < hide2_num_; ++i)
		delete[] hide2_weight_[i];
	delete[] hide2_weight_;
	delete[]  hide2_error_;
	delete[]  hide2_output_;

	for (int i = 0; i < output_num_; ++i)
		delete[] output_weight_[i];
	delete[] output_weight_;
	delete[]  output_error_;
	delete[] output_;
}

const double* BackPropagation::predict(double* input)
{
	forward(input);
	return output_;
}

void BackPropagation::train(double** samples, double** results, int sample_num, double threshold)
{
	while (error_ > threshold)	{
		std::cout << "Train Error:" << error_ << std::endl;
		error_ = 0.0;
		for (int i = 0; i < sample_num; ++i) {
			forward(samples[i]);
			backward(results[i]);
		}	
	}	
}

void BackPropagation::initWeight()
{
	for (int i = 0; i < hide1_num_; ++i) {
		for (int j = 0; j < (input_num_ + 1); ++j)
			hide1_weight_[i][j] = getRandomWieght();
	}
	for (int i = 0; i < hide2_num_; ++i) {
		for (int j = 0; j < (hide1_num_ + 1); ++j)
			hide2_weight_[i][j] = getRandomWieght();
	}
	for (int i = 0; i < output_num_; ++i) {
		for (int j = 0; j < hide2_num_ + 1; ++j)
			output_weight_[i][j] = getRandomWieght();
	}		
}

void BackPropagation::forward(double* input)
{   
	//save input
	input_[0] = 1.0;
	for (int i = 0; i < input_num_; ++i) 
		input_[i + 1] = input[i];

	//input layer -> hide1 layer
	hide1_output_[0] = 1.0;
	for (int i = 0; i < hide1_num_; ++i) {
		hide1_output_[i + 1] = 0.0;
		for (int j = 0; j < input_num_ + 1; ++j)
			hide1_output_[i + 1] += input_[j] * hide1_weight_[i][j];
		hide1_output_[i + 1] = sigmoid(hide1_output_[i + 1]);
	}
	//hide1 layer -> hide2 layer
	hide2_output_[0] = 1.0;
	for (int i = 0; i < hide2_num_; ++i) {
		hide2_output_[i + 1] = 0.0;
		for (int j = 0; j < hide1_num_ + 1; ++j)
			hide2_output_[i + 1] += hide1_output_[j] * hide2_weight_[i][j];
		hide2_output_[i + 1] = sigmoid(hide2_output_[i + 1]);
	}
	//hide2 layer -> output layer
	for (int i = 0; i < output_num_; ++i) {
		output_[i] = 0.0;
		for (int j = 0; j < hide2_num_ + 1; ++j)
			output_[i] += hide2_output_[j] * output_weight_[i][j];
		output_[i] = sigmoid(output_[i]);
	}
}

void BackPropagation::backward(double* output)
{   
    // ÇóÎó²îÏî
	for (int i = 0; i < output_num_; ++i) {
		output_error_[i] = (output[i] - output_[i]) * output_[i] * (1.0 - output_[i]);
		double temp_error = fabs(output[i] - output_[i]);
		error_ += temp_error * temp_error / 2.0;
	}
	for (int i = 0; i < hide2_num_; ++i) {
		double sum_error = 0.0;  //Îó²î
		for (int j = 0; j < output_num_; ++j)
			sum_error += output_weight_[j][i] * output_error_[j];
		hide2_error_[i] = sum_error * hide2_output_[i + 1] * (1.0 - hide2_output_[i + 1]);
	}
	for (int i = 0; i < hide1_num_; ++i) {
		double sum_error = 0.0; //Îó²î
		for (int j = 0; j < hide2_num_; ++j)
			sum_error += hide2_weight_[j][i] * hide2_error_[j];
		hide1_error_[i] = sum_error * hide1_output_[i + 1] * (1.0 - hide1_output_[i + 1]);
	}

	//update hide1 weights
	for (int i = 0; i < hide1_num_; ++i){
		for (int j = 0; j < input_num_ + 1; ++j) 
			hide1_weight_[i][j] += rate_ * hide1_error_[i] * input_[j];		
	}
	//update hide2 weights
	for (int i = 0; i < hide2_num_; ++i) {
		for (int j = 0; j < hide1_num_ + 1; ++j)
			hide2_weight_[i][j] += rate_ * hide2_error_[i] * hide1_output_[j];
	}
	//update output weights
	for (int i = 0; i < output_num_; ++i) {
		for (int j = 0; j < hide2_num_ + 1; ++j)
			output_weight_[i][j] += rate_ * output_error_[i] * hide2_output_[j];
	}
}
