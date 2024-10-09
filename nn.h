#pragma once
#include "matrix.h"
#include <initializer_list>
#include <vector>

class NeuralNetwork {
private:
	std::vector<size_t> layers_size;
	std::vector<Matrix> weights;
	std::vector<Matrix> biases;
	std::vector<Matrix> values;

public:
	size_t size() const;
	size_t layer_size(size_t i) const;

	NeuralNetwork(const std::initializer_list<size_t>& init);
	Matrix forward(const Matrix& input);
	float cost(const Matrix& input, const Matrix& output);
	~NeuralNetwork();
};
