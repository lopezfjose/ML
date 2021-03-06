// ml-dev-client.cpp : Defines the entry point for the console application.

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>

#include <ml/ML.hxx>

void PrintDoublePointerMatrix(double **m) {
	for (auto i = 0; i < N; i++) {
		for (auto j = 0; j < N; j++) {
			std::cout << m[i][j] << ' ';
		}

		std::cout << '\n';
	}
}

void PrintMatrix(double m[N][N]) {
	for (auto i = 0; i < N; i++) {
		for (auto j = 0; j < N; j++) {
			std::cout << m[i][j] << ' ';
		}

		std::cout << '\n';
	}
}


int main()
{
	srand(time(NULL));

	double matrix[N][N] = { 0 };

	for (auto i = 0; i < N; i++) {
		for (auto j = 0; j < N; j++) {
			matrix[i][j] = rand() % 100 + 1;
		}
	}

	auto r = ML::dmatrix(1, N, 1, N);

	PrintMatrix(matrix);
	PrintDoublePointerMatrix(r);

	ML::GaussJordanElimination(matrix, N, r, N);
}

