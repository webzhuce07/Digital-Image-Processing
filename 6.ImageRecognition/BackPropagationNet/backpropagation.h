#ifndef __backpropagation_h__
#define __backpropagation_h__

//input layer + two hide layer + output layer
class BackPropagation
{
public:
	BackPropagation(int input_num = 2, int hide1_num = 3, int hide2_num = 2, int output_num = 1);
	~BackPropagation();
	const double* predict(double* input);
	void train(double** samples, double** results, int sample_num, double threshold);

private:
	void initWeight();
	void forward(double* input);
	void backward(double* output);  // update weights
	int input_num_ = 2; 
	int hide1_num_ = 3;
	int hide2_num_ = 2;
	int output_num_ = 1;
	double rate_ = 0.9;
	double* input_; //input_[input_num_ + 1], input_[0] = 1.0
	double** hide1_weight_ = nullptr; //hide1_weight_[hide1_num_][input_num_ + 1],hide1_weight_[hide1_num_][0] is bias
	double*  hide1_error_ = nullptr;//hide1_error_[hide1_num_]
	double* hide1_output_ = nullptr; //hide1_output_[hide1_num_ + 1],hide1_output_[0] = 1.0
	double** hide2_weight_ = nullptr; //hide2_weight_[hide2_num_][hide1_num_ + 1], hide2_weight_[hide2_num_][0] is bias
	double*  hide2_error_ = nullptr;//hide2_error_[hide2_num_];
	double*  hide2_output_ = nullptr; //hide2_output_[hide2_num_ + 1],hide2_output_[0] = 1.0
	double** output_weight_ = nullptr; //output_weight_[output_num_][hide2_num_ + 1],output_weight_[output_num_][0] is bias
	double*  output_error_ = nullptr;//output_error_[output_num_];
	double* output_ = nullptr;//output_[output_num_]
	double error_ = 100.0;
};

#endif
