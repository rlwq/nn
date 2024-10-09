#include "matrix.h"
#include "nn.h"
#include <iostream>

int main () {
	NeuralNetwork xor_model = {2, 3, 1};
	Matrix m(3, 4, 5);

	std::cout << m << std::endl;
	std::cout << m.get_rows() << " x " << m.get_cols() << std::endl;
	std::cout << xor_model.size() << std::endl;
	return 0;
}
