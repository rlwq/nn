#pragma once
#include "matrix.h"
#include <initializer_list>
#include <vector>
#include <iostream>

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
	float cost(const Matrix& input_batch, const Matrix& output_batch);
	void finite_dist(const Matrix& input_batch, const Matrix& output_batch,
									 float rate, float eps);
	~NeuralNetwork();

	friend std::ostream& operator << (std::ostream& out, const NeuralNetwork& nn);
};

std::ostream& operator << (std::ostream& out, const NeuralNetwork& nn);
