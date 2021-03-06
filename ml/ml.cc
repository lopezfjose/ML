
#include <ml/ML.hxx>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#else
#error "Windows is the only currently-supported operating system"
#endif

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
	
	// TODO: Set up DllMain()

	switch (ul_reason_for_call) {
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
	}

	return TRUE;
}

#define NR_END 1

namespace ML {
	__declspec(dllexport) double **dmatrix(long nrl, long nrh, long ncl, long nch) {
		int rows = nrh - nrl + 1;
		int cols = nch - ncl + 1;

		double **m = static_cast<double **>(malloc((std::size_t) ((rows + NR_END) * sizeof(int *))));

		if (!m) {
			std::cerr << "Failed to allocate matrix!\n";

			exit(EXIT_FAILURE);
		}

		m += NR_END;
		m -= nrl;

		// Allocate rows and set pointers to them
		m[nrl] = static_cast<double *>(malloc((std::size_t) ((rows * cols + NR_END) * sizeof(int))));

		if (!m[nrl]) {
			std::cerr << "Failed to allocate matrix (2)!\n";

			exit(EXIT_FAILURE);
		}

		m[nrl] += NR_END;
		m[nrl] -= ncl;

		for (auto i = nrl + 1; i <= nrh; i++) {
			m[i] = m[i - 1] + cols;
		}

		// Return the pointer to array of pointers to rows
		return m;
	}

	__declspec(dllexport) double* dvector(long nl, long nh) {
		// Allocate an int vector with subscript range v[nl..nh]
		double *v = static_cast<double *>(malloc((std::size_t) ((nh - nl + 1 + NR_END) * sizeof(int))));

		if (!v) {
			std::cerr << "Filed to allocate vector...\n";

			exit(EXIT_FAILURE);
		}

		return (v - nl + NR_END);
	}

	__declspec(dllexport) int* ivector(long nl, long nh) {
		// Allocate an int vector with subscript range v[nl..nh]
		int *v = static_cast<int *>(malloc((std::size_t) ((nh - nl + 1 + NR_END) * sizeof(int))));

		if (!v) {
			std::cerr << "Filed to allocate vector...\n";

			exit(EXIT_FAILURE);
		}

		return (v - nl + NR_END);
	}

	__declspec(dllexport) void GaussJordanElimination(double a[N][N], int n, double **b, int m) {
		int row = 0;
		int col = 0;

		double pivotInverse = 0.0;

		int *colIndex = ivector(1, n);
		int *rowIndex = ivector(1, n);

		int *pivot = ivector(1, n);

		for (auto j = 1; j <= n; j++) {
			pivot[j] = 0;
		}

		for (auto i = 1; i <= n; i++) {
			auto big = 0.0;

			for (auto j = 1; j <= n; j++) {
				if (pivot[j] != 1) {
					for (auto k = 1; k <= n; k++) {
						if (pivot[k] == 0) {
							if (fabs(a[j][k]) >= big) {
								big = fabs(a[j][k]);

								row = j;
								col = k;
							}
						} else if (pivot[k] > 1) {
							std::cerr << "Singular Matrix; cannot reduce!\n";

							exit(EXIT_SUCCESS);
						}
					}
				}
			}

			++(pivot[col]);

			if (row != col) {
				for (auto l = 1; l <= n; l++) std::swap(a[row][l], a[col][l]);
				for (auto l = 1; l <= m; l++) std::swap(b[row][l], b[col][l]);
			}

			rowIndex[i] = row;
			colIndex[i] = col;

			if (a[col][col] == 0.0) {
				std::cerr << "Singular Matrix (2); cannot reduce!\n";

				exit(EXIT_SUCCESS);
			}

			pivotInverse = 1.0 / a[col][col];
			a[col][col] = 1.0;

			for (auto l = 1; l <= n; l++) a[col][l] *= pivotInverse;
			for (auto l = 1; l <= m; l++) b[col][l] *= pivotInverse;

			for (auto ll = 1; ll <= n; ll++) {
				if (ll != col) {
					double dummyVal = a[ll][col];
					
					a[ll][col] = 0.0;

					for (auto l = 1; l <= n; l++) a[ll][l] -= a[col][l] * dummyVal;
					for (auto l = 1; l <= m; l++) b[ll][l] -= b[col][l] * dummyVal;
				}
			}
		} // End main loop

		for (auto l = n; l >= 1; l--) {
			if (rowIndex[l] != colIndex[l]) {
				for (auto k = 1; k <= n; k++) {
					std::swap(a[k][rowIndex[l]], a[k][colIndex[l]]);
				}
			}
		} // Finished

		// free_ivector(pivot, 1, n);
		// free_ivector(rowIndex, 1, n);
		// free_ivector(colIndex, 1, n);
	}
}
