/*************************************************
Copyright:QZStech
Author:chengmo
Date:2017-11-21
Description:calc the Frechet Distance (3D)
**************************************************/
#include "FrechetDistance.h"
#include <iostream>
#include <sstream>
#include <fstream>
using namespace std;

FrechetDistance3D::FrechetDistance3D()
{
	ones_array_.clear();
	first_time_ = true;
}

FrechetDistance3D::~FrechetDistance3D()
{
}

double FrechetDistance3D::frechetDistance(vector<vector<double>> &P1, vector<vector<double>> &P2)
{
	vector<double> x1, x2, y1, y2, z1, z2;
	double result = -1;
	//check vector length
	size_t length = P1.size() > P2.size() ? (size_t)P2.size() : (size_t)P1.size();	
	if (length<=0)
	{
		return -1.0;
	}

	//get path point length
	for (size_t index_1 = 0; index_1 < P1.size(); index_1++)
	{
		x1.push_back(P1[index_1][0]);
		y1.push_back(P1[index_1][1]);
		z1.push_back(P1[index_1][2]);
		
	}
	for (size_t index_2 = 0; index_2 < P2.size(); index_2++)
	{
		x2.push_back(P2[index_2][0]);
		y2.push_back(P2[index_2][1]);
		z2.push_back(P2[index_2][2]);
	}

	//calculate frechet distance matrix
	vector<vector<double>> frechet_dis_matrix;
	double f_max = std::numeric_limits<double>::min();
	double f_min = std::numeric_limits<double>::max();
	double distance = 0;
	vector<double> frechet_dis_row;
	for (size_t index_row = 0; index_row < P1.size(); index_row++)
	{			
		for (size_t index_column = 0; index_column < P2.size(); index_column++)
		{
			distance = sqrt(pow(x1[index_row] - x2[index_column], 2) + pow(y1[index_row] - y2[index_column], 2) + pow(z1[index_row] - z2[index_column], 2));
			if (distance<f_min)
			{
				f_min = distance;
			}
			if (distance>f_max)
			{
				f_max = distance;
			}
			frechet_dis_row.push_back(distance);
		}
		frechet_dis_matrix.push_back(frechet_dis_row);
		frechet_dis_row.clear();
	}

	//compute frechet distance
	double res = (f_max - f_min) / 100;

	//get region number of beginning and end points
	vector<vector<double>> frechet_compare_matrix;
	for (double range = f_min; range < f_max; range += res)
	{		
		compareMatrix(frechet_dis_matrix, frechet_compare_matrix, range);
		/*if (judgeconnected(frechet_compare_matrix))
		{
			result = range;
			break;
		}*/
		vector<vector<double>> frechet_group_mat = bwlabel(frechet_compare_matrix);
		if (frechet_group_mat[0][0] != 0 && frechet_group_mat[0][0] == (frechet_group_mat.back()).back())
		{
 			result = range;
			break;
		}
		frechet_compare_matrix.clear();
	}
	if (result == -1)
	{
		result = f_max;
	}
	result = getRatio(result, f_max, f_min);
	return result;
}

bool FrechetDistance3D::judgeconnected(vector<vector<double>> frechet_compare_matrix)
{
	vector<double> temp_array(frechet_compare_matrix[0].size(),0);
	temp_array[0] = 1;
	if (!(frechet_compare_matrix[0][0] == 1 && frechet_compare_matrix[frechet_compare_matrix.size()-1][frechet_compare_matrix[0].size()-1] == 1))
	{
		return false;
	}

	for (size_t index_row = 0; index_row < frechet_compare_matrix.size(); index_row++)
	{
		if (compareArray(temp_array, frechet_compare_matrix[index_row]))
		{
			temp_array = frechet_compare_matrix[index_row];
		}
		else
		{
			ones_array_.clear();
			return false;
		}
	}
	ones_array_.clear();
	return true;
}

void FrechetDistance3D::compareMatrix(vector<vector<double>>& origin_matrix, vector<vector<double>>& result_matrix, double & range)
{
	vector<double> frechet_compare_row;
	for (size_t index_row = 0; index_row < origin_matrix.size(); index_row++)
	{
		for (size_t index_column = 0; index_column < origin_matrix[index_row].size(); index_column++)
		{
			if (origin_matrix[index_row][index_column]<=range)
			{
				frechet_compare_row.push_back(1);
			}
			else
			{
				frechet_compare_row.push_back(0);
			}
		}
		result_matrix.push_back(frechet_compare_row);
		frechet_compare_row.clear();
	}
}

bool FrechetDistance3D::compareArray(vector<double> &temp_array, vector<double> &curr_array)
{
	size_t length = temp_array.size();
	calcOnesArray(temp_array);
	for (size_t index = 0; index < ones_array_.size(); index++)
	{
		if (curr_array[ones_array_[index]])
		{
			return true;
		}
		else if ((ones_array_[index] + 1)<curr_array.size() && curr_array[ones_array_[index]+1])
		{
			return true;
		}
		else if ((ones_array_[index] - 1)>0 )
		{
			if (curr_array[ones_array_[index] - 1])
			{
				return true;
			}		
		}
	}
	return false;
}

void FrechetDistance3D::calcOnesArray(vector<double>& array_1)
{
	vector<int> next_ones_array;
	if (first_time_)
	{
		ones_array_.push_back(0);		
		first_time_ = false;
	}
	for (size_t index = 0; index < ones_array_.size(); index++)
	{
		if (array_1[ones_array_[index]] == 1)
		{
			next_ones_array.push_back(ones_array_[index]);
		}
	}
	while ((next_ones_array.size() >= 1) && array_1[next_ones_array[0]])
	{
		if (((next_ones_array[0])>0) && array_1[next_ones_array[0] - 1])
		{
			next_ones_array.insert(next_ones_array.begin(), next_ones_array[0] - 1);
		}
		else
		{
			break;
		}
	}
	while ((next_ones_array.size()>=1) && array_1[next_ones_array[next_ones_array.size()-1]])
	{
		if (((next_ones_array[next_ones_array.size() - 1] + 1)<array_1.size()) && array_1[next_ones_array[next_ones_array.size() - 1]+1])
		{
			next_ones_array.push_back(next_ones_array[next_ones_array.size() - 1] + 1);
		}
		else
		{
			break;
		}
	}
	sort(next_ones_array.begin(), next_ones_array.end());
	next_ones_array.erase(unique(next_ones_array.begin(), next_ones_array.end()), next_ones_array.end());
	ones_array_ = next_ones_array;

	first_time_ = true;
}


double FrechetDistance3D::getRatio(double result, double f_max, double f_min)
{
	double probablity = (1-normcdf((result-f_min)/(f_max/3)))*2;
	return probablity;
}

vector<vector<double>> FrechetDistance3D::bwlabel(vector<vector<double>> frechet_compare_mat)
{
	vector<vector<double>> frechet_group_mat;
	vector<vector<int>> group_num;
	
	group_num = number_of_group(frechet_compare_mat, frechet_group_mat);
	group_mat_process(group_num, frechet_group_mat);
	group_num_change(group_num, frechet_group_mat);

	
	size_t length_row = frechet_group_mat.size();
	size_t length_column = frechet_group_mat[0].size();

	std::fstream f_result_replay_;
	std::fstream f_campare_replay_;
	f_result_replay_.close();
	f_result_replay_.open("E:/Project/cm_project/FrechetDistance/frechet_group_mat.txt", std::ios::out);
	f_result_replay_.open("E:/Project/cm_project/FrechetDistance/frechet_campare_mat.txt", std::ios::out);
	for (int i = 0; i < length_row; i++)
	{
		for (int j = 0; j < length_column; j++)
		{
			f_result_replay_ << frechet_group_mat[i][j] <<  " " ;
			if (j == length_column-1)
			{
				f_result_replay_ << endl;
			}
		}
	}
	f_result_replay_.close();
	return frechet_group_mat;
}

vector<vector<int>> FrechetDistance3D::number_of_group(vector<vector<double>> frechet_compare_mat, vector<vector<double>> &frechet_group_mat)
{
	size_t length_row = frechet_compare_mat.size();
	size_t length_column = frechet_compare_mat[0].size();
	vector<double> temp_group_mat;
	vector<vector<int>> group_num_mat;
	int group_num = 0;
	for (size_t index_row = 0; index_row < length_row; index_row++)
	{
		if (frechet_compare_mat[index_row][0]==1){
			group_num++;
			group_num_mat.push_back({ group_num });
			temp_group_mat.push_back(group_num);
		}
		else{
			temp_group_mat.push_back(0);
		}
		for (size_t index_column = 1; index_column < length_column; index_column++)
		{
			if (frechet_compare_mat[index_row][index_column]==1 && frechet_compare_mat[index_row][index_column-1] == 0)
			{
				group_num++;
				group_num_mat.push_back({ group_num });
				temp_group_mat.push_back(group_num);
			}
			else if (frechet_compare_mat[index_row][index_column] == 1 && frechet_compare_mat[index_row][index_column - 1] == 1)
			{
				temp_group_mat.push_back(group_num);
			}
			else if (frechet_compare_mat[index_row][index_column] == 0)
			{
				temp_group_mat.push_back(0);
			}
		}
		frechet_group_mat.push_back(temp_group_mat);
		temp_group_mat.clear();
	}
	return group_num_mat;
}

void FrechetDistance3D::group_mat_process(vector<vector<int>>& number_of_group, vector<vector<double>>& frechet_group_mat)
{
	size_t length_row = frechet_group_mat.size();
	size_t length_column = frechet_group_mat[0].size();
	size_t length_num = number_of_group.size();

	for (size_t index_row = 1; index_row < length_row; index_row++)
	{
		group_row_process(frechet_group_mat[index_row - 1], frechet_group_mat[index_row], number_of_group);
	}

}

void FrechetDistance3D::group_row_process(vector<double>& previous_row, vector<double>& curr_row, vector<vector<int>>& number_of_group)
{
	size_t length_column = previous_row.size();
	int last_num=0;
	for (size_t index_column = 0; index_column < length_column; index_column++)
	{
		if (previous_row[index_column] ){
			vector<int> neighbor_vector = findNeighbor(index_column, curr_row);
			for (int neighbor_flag : neighbor_vector)
			{
				if (neighbor_flag != -1) {
					/*if (index_column >= neighbor_flag)*/
					{
						if (number_of_group[curr_row[neighbor_flag] - 1].size() <= 1)
						{
							number_of_group[curr_row[neighbor_flag] - 1].push_back(number_of_group[previous_row[index_column] - 1].back());
						}
						else if (number_of_group[curr_row[neighbor_flag] - 1].back()< number_of_group[previous_row[index_column] - 1].back())
						{
							num_trace(number_of_group, number_of_group[previous_row[index_column] - 1].back(), number_of_group[curr_row[neighbor_flag] - 1].back());
							number_of_group[previous_row[index_column] - 1].push_back(number_of_group[curr_row[neighbor_flag] - 1].back());
							
						}
						else if (number_of_group[curr_row[neighbor_flag] - 1].back()> number_of_group[previous_row[index_column] - 1].back())
						{
							number_of_group[curr_row[neighbor_flag] - 1].push_back(number_of_group[previous_row[index_column] - 1].back());
						}
					}
				}
			}
		}
	}
}

vector<int> FrechetDistance3D::findNeighbor(size_t index, vector<double>& neighbor_row)
{
	vector<int> result;
	if (index == 0)
	{
		if (neighbor_row[0])
		{
			result.push_back(0);
		}
		if (neighbor_row[1])
		{
			result.push_back(1);
		}
	}
	else if (index == neighbor_row.size()-1)
	{
		if (neighbor_row[neighbor_row.size() - 2])
		{
			result.push_back(neighbor_row.size() - 2);
		}
		else if (neighbor_row[neighbor_row.size() - 1])
		{
			result.push_back(neighbor_row.size() - 1);
		}
	}
	else
	{
		if (neighbor_row[index - 1])
		{
			result.push_back(index - 1);
		}
		else if(neighbor_row[index])
		{
			result.push_back(index);
		}
		else if (neighbor_row[index + 1])
		{
			result.push_back(index + 1);
		}
	}
	return result;
}

void FrechetDistance3D::group_num_change(vector<vector<int>>& number_of_group, vector<vector<double>>& frechet_group_mat)
{
	size_t length_num = number_of_group.size();
	size_t length_row = frechet_group_mat.size();
	size_t length_column = frechet_group_mat[0].size();
	for (int index_num = length_num-1; index_num >=0; index_num--)
	{
		//反向遍历group_num，直到索引的标志size不为1，持续寻找
		//找到后，将该Index_num全部改为终止标志
		int trace_index = index_num;
		int curr_num = number_of_group[trace_index][0],trace_num = curr_num;
	
		while (number_of_group[trace_index].size()!=1)
		{
			trace_num = number_of_group[trace_index].back();
			trace_index = number_of_group[trace_index].back()-1;
			unique_vector(number_of_group[trace_index]);
		}
		trace_num = number_of_group[trace_index].back();

		for (size_t index_row = 0; index_row <length_row; index_row++)
		{
			for (size_t index_column = 0; index_column <length_column; index_column++)
			{
				if (frechet_group_mat[index_row][index_column] == curr_num)
				{
					frechet_group_mat[index_row][index_column] = trace_num;
				}
			}
		}
	}
}

void FrechetDistance3D::num_trace(vector<vector<int>>& number_of_group, int pre_num,int tar_num)
{
	for (int i = 0;i < number_of_group[pre_num-1].size();i++)
	{
		int num = number_of_group[pre_num - 1][i];
		number_of_group[num-1].push_back(tar_num);
	}
}

void FrechetDistance3D::unique_vector(vector<int>& num_array)
{
	vector<int> new_array;
	int last_num = num_array[0];
	new_array.push_back(last_num);
	for (size_t i = 0; i < num_array.size(); i++)
	{
		if (num_array[i]!= last_num)
		{
			last_num = num_array[i];
			bool add_flag = true;
			for (size_t i = 0; i < new_array.size(); i++)
			{
				if (last_num == new_array[i])
				{
					add_flag = false;
				}
			}
			if (add_flag)
			{
				new_array.push_back(last_num);
			}
			
		}
	}
	num_array = new_array;
}







