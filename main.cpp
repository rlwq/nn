#include "matrix.h"
#include "nn.h"
#include <iostream>

int main () {
	srand(time(0));
	Matrix ins = {{0, 0, 0},
								{0, 0, 1},
								{0, 1, 0},
								{0, 1, 1},
								// {1, 0, 0},
								{1, 0, 1},
								{1, 1, 0},
								{1, 1, 1}
	};
	Matrix outs = {{0, 0},
								 {0, 1},
								 {0, 1},
								 {1, 0},
								 // {0, 1},
								 {1, 0},
								 {1, 0},
								 {1, 1}
	};
	NeuralNetwork or_model = {3, 2, 2, 2};

	// std::cout << or_model << std::endl;

	float c = 1000;
	for (size_t i = 0; c >= 0.04; i++) {
		for (int j = 0; j < 2000; j++)
			or_model.finite_dist(ins, outs, 1e-3, 1e-4);
		c = or_model.cost(ins, outs);
	  std::cout << i << ": " << c << std::endl;
	}

	std::cout << "--------------------" << std::endl;
	for (size_t i = 0; i < ins.rows(); i++)
		std::cout << ins.get_row(i) << " -> "
							<< or_model.forward(ins.get_row(i)) << std::endl;
	std::cout << "--------------------" << std::endl;
	std::cout << "[1 0 0] -> " << or_model.forward({1, 0, 0}) << std::endl;
	return 0;
}
