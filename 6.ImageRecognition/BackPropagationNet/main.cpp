#include "backpropagation.h"
#include "backpropagation2.h"
#include <iostream>

int main()
{
	BackPropagation2 bp;
	//BackPropagation bp;
	int sample_num = 4;
	double** inputx = new double*[sample_num];
	for (int i = 0; i < sample_num; ++i) 
		inputx[i] = new double[2];
	inputx[0][0] = 0.5;
	inputx[0][1] = 0.5;
	inputx[1][0] = 0.0;
	inputx[1][1] = 0.1;
	inputx[2][0] = 0.2;
	inputx[2][1] = 0.4;
	inputx[3][0] = 0.1;
	inputx[3][1] = 0.1;
	double** outputx = new double*[sample_num];
	for (int i = 0; i < sample_num; ++i)
		outputx[i] = new double[1];
	outputx[0][0] = 1.0;
	outputx[1][0] = 0.1;
	outputx[2][0] = 0.6;
	outputx[3][0] = 0.2;

	double threshold = 0.0001;
	bp.train(inputx, outputx, sample_num, threshold);

	int test_num = 4;
	double** input = new double*[test_num];
	for (int i = 0; i < test_num; ++i)
		input[i] = new double[2];
	input[0][0] = 0.1;
	input[0][1] = 0.2;
	input[1][0] = 0.15;
	input[1][1] = 0.9;
	input[2][0] = 1.1;
	input[2][1] = 0.01;
	input[3][0] = 0.88;
	input[3][1] = 1.03;
	
	for (int i = 0; i < test_num; ++i){
		const double*r = bp.predict(input[i]);
	std::cout << r[0] << std::endl;
    }
	system("pause");
	for (int i = 0; i < sample_num; ++i)
		delete[] inputx[i];
	delete[] inputx;
	for (int i = 0; i < sample_num; ++i)
		delete[] outputx[i];
	delete outputx;
	for (int i = 0; i < test_num; ++i)
		delete[] input[i];
	delete[] input;
	return 0;
}