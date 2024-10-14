#include "matrix.h"
#include "nn.h"
#include <iostream>
#include <cmath>

int main () {
	srand(time(0));
	NeuralNetwork nn = {3, 40, 40, 40, 40, 40, 40, 40, 40, 2};

	Matrix inps = {{0, 0, 0},
								 {0, 0, 1},
								 {0, 1, 0},
								 {0, 1, 1},
								 {1, 0, 0},
								 {1, 0, 1},
								 {1, 1, 0},
								 {1, 1, 1}};

	Matrix outs = {{0, 0},
								 {0, 1},
								 {0, 1},
								 {1, 0},
								 {0, 1},
								 {1, 0},
								 {1, 0},
								 {1, 1}};

	float C = 10;
	size_t i = 0;

	std::cout << nn.batch_cost(inps, outs) << std::endl;

	while (C > 0.001) {
		for (int j = 0; j < inps.rows(); j++)
			nn.backward(inps.get_row(j), outs.get_row(j), 0.1);
		C = nn.batch_cost(inps, outs);
		std::cout << C << std::endl;
	}

	for (int i = 0; i < inps.rows(); i++)
		std::cout << inps.get_row(i) << " -> " << nn.forward(inps.get_row(i)) << std::endl;
	return 0;
}
