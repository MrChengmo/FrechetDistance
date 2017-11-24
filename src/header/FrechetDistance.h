#include <math.h>
#include <vector>
#include <iostream>
#include "algorithm"
#define M_PI       3.14159265358979323846
using namespace std;

class FrechetDistance3D
{
public:
	FrechetDistance3D();
	~FrechetDistance3D();
	double frechetDistance(vector<vector<double>> &P1, vector<vector<double>> &P2);
	bool judgeconnected(vector<vector<double>> frechet_compare_matrix);
	void compareMatrix(vector<vector<double>> &origin_matrix, vector<vector<double>> &result_matrix, double &range);
	bool compareArray(vector<double> &array_1, vector<double> &array_2);
	void calcOnesArray(vector<double> &array_1);
	double getRatio(double result, double f_max, double f_min);
	vector<vector<double>> bwlabel(vector<vector<double>> frechet_compare_mat);
	vector<vector<int>> number_of_group(vector<vector<double>> frechet_compare_mat , vector<vector<double>> &frechet_group_mat);
	void group_mat_process(vector<vector<int>> &number_of_group, vector<vector<double>> &frechet_group_mat);
	void group_row_process(vector<double> &previous_row, vector<double> &curr_row, vector<vector<int>> &number_of_group);
	vector<int> findNeighbor(size_t index, vector<double> &neighbor_row);
	void group_num_change(vector<vector<int>>& number_of_group, vector<vector<double>>& frechet_group_mat);
	void num_trace(vector<vector<int>>& number_of_group, int pre_num,int tar_num);
	void unique_vector(vector<int> &num_array);
private:
	vector<int> ones_array_;
	bool first_time_;
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


