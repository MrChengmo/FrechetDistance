#include <math.h>
#include <vector>
#include <iostream>
#include "algorithm"
#include "Astar.h"
#include "Bwlabel.h"
#define M_PI       3.14159265358979323846
using namespace std;

class FrechetDistance3D
{
public:
	FrechetDistance3D();
	~FrechetDistance3D();
	double frechetDistance(vector<vector<double>> &P1, vector<vector<double>> &P2);
	void compareMatrix(vector<vector<double>> &origin_matrix, vector<vector<int>> &result_matrix, double &range,int mode);
	double getRatio(double result, double f_max, double f_min);
private:
	double normcdf(double x)
	{
		// constants
		double a1 = 0.254829592;
		double a2 = -0.284496736;
		double a3 = 1.421413741;
		double a4 = -1.453152027;
		double a5 = 1.061405429;
		double p = 0.3275911;

		// Save the sign of x
		int sign = 1;
		if (x < 0)
			sign = -1;
		x = fabs(x) / sqrt(2.0);

		// A&S formula 7.1.26
		double t = 1.0 / (1.0 + p*x);
		double y = 1.0 - (((((a5*t + a4)*t) + a3)*t + a2)*t + a1)*t*exp(-x*x);

		return 0.5*(1.0 + sign*y);
	}

};


