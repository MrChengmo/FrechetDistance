#pragma once
#include <math.h>
#include <vector>
using namespace std;

class BwLbel
{
public:
	BwLbel();
	~BwLbel();
	vector<vector<int>> bwlabel(vector<vector<int>> frechet_compare_mat);
	vector<vector<int>> number_of_group(vector<vector<int>> frechet_compare_mat, vector<vector<int>> &frechet_group_mat);
	void group_mat_process(vector<vector<int>> &number_of_group, vector<vector<int>> &frechet_group_mat);
	void group_row_process(vector<int> &previous_row, vector<int> &curr_row, vector<vector<int>> &number_of_group);
	vector<int> findNeighbor(size_t index, vector<int> &neighbor_row);
	void group_num_change(vector<vector<int>>& number_of_group, vector<vector<int>>& frechet_group_mat);
	void num_trace(vector<vector<int>>& number_of_group, int pre_num, int tar_num);
	void unique_vector(vector<int> &num_array);
	void num_change(vector<vector<int>>& number_of_group, int origin_num, int target_num);
private:

};




