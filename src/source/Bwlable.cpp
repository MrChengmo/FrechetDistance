/*************************************************
Copyright:QZStech
Author:chengmo
Date:2017-11-27
Description:Matlab function bwlabel using C++ apply
**************************************************/
#include "Bwlabel.h"
#include <sstream>
#include <fstream>

BwLbel::BwLbel()
{
}

BwLbel::~BwLbel()
{
}

vector<vector<int>> BwLbel::bwlabel(vector<vector<int>> frechet_compare_mat)
{
	vector<vector<int>> frechet_group_mat;
	vector<vector<int>> group_num;

	group_num = number_of_group(frechet_compare_mat, frechet_group_mat);
	group_mat_process(group_num, frechet_group_mat);
	group_num_change(group_num, frechet_group_mat);

	//size_t length_row = frechet_group_mat.size();
	//size_t length_column = frechet_group_mat[0].size();
	//std::fstream f_result_replay_;
	//std::fstream f_campare_replay_;
	//f_result_replay_.close();
	//f_result_replay_.open("E:/Project/cm_project/FrechetDistance/frechet_group_mat.txt", std::ios::out);
	//for (int i = 0; i < length_row; i++)
	//{
	//	for (int j = 0; j < length_column; j++)
	//	{
	//		f_result_replay_ << frechet_group_mat[i][j] << " ";
	//		if (j == length_column - 1)
	//		{
	//			f_result_replay_ << endl;
	//		}
	//	}
	//}
	//f_result_replay_.close();

	return frechet_group_mat;
}

vector<vector<int>> BwLbel::number_of_group(vector<vector<int>> frechet_compare_mat, vector<vector<int>> &frechet_group_mat)
{
	size_t length_row = frechet_compare_mat.size();
	size_t length_column = frechet_compare_mat[0].size();
	vector<int> temp_group_mat;
	vector<vector<int>> group_num_mat;
	int group_num = 0;
	for (size_t index_row = 0; index_row < length_row; index_row++)
	{
		if (frechet_compare_mat[index_row][0] == 1) {
			group_num++;
			group_num_mat.push_back({ group_num });
			temp_group_mat.push_back(group_num);
		}
		else {
			temp_group_mat.push_back(0);
		}
		for (size_t index_column = 1; index_column < length_column; index_column++)
		{
			if (frechet_compare_mat[index_row][index_column] == 1 && frechet_compare_mat[index_row][index_column - 1] == 0)
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

void BwLbel::group_mat_process(vector<vector<int>>& number_of_group, vector<vector<int>>& frechet_group_mat)
{
	size_t length_row = frechet_group_mat.size();
	size_t length_column = frechet_group_mat[0].size();
	size_t length_num = number_of_group.size();

	for (size_t index_row = 1; index_row < length_row; index_row++)
	{
		group_row_process(frechet_group_mat[index_row - 1], frechet_group_mat[index_row], number_of_group);
	}

}

void BwLbel::group_row_process(vector<int>& previous_row, vector<int>& curr_row, vector<vector<int>>& number_of_group)
{
	size_t length_column = previous_row.size();
	int last_num = 0;
	for (size_t index_column = 0; index_column < length_column; index_column++)
	{
		if (previous_row[index_column]) {
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

vector<int> BwLbel::findNeighbor(size_t index, vector<int>& neighbor_row)
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
	else if (index == neighbor_row.size() - 1)
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
		else if (neighbor_row[index])
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

void BwLbel::group_num_change(vector<vector<int>>& number_of_group, vector<vector<int>>& frechet_group_mat)
{
	size_t length_num = number_of_group.size();
	size_t length_row = frechet_group_mat.size();
	size_t length_column = frechet_group_mat[0].size();
	for (int index_num = length_num - 1; index_num >= 0; index_num--)
	{
		//反向遍历group_num，直到索引的标志size不为1，持续寻找
		//找到后，将该Index_num全部改为终止标志
		int trace_index = index_num;
		int curr_num = number_of_group[trace_index][0], trace_num = curr_num;

		while (number_of_group[trace_index].size() != 1)
		{
			trace_num = number_of_group[trace_index].back();
			trace_index = number_of_group[trace_index].back() - 1;
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
	for (int index_row = length_row - 1; index_row > 0; index_row--)
	{
		for (int index_column = length_column - 2; index_column > 0; index_column--)
		{
			if (frechet_group_mat[index_row][index_column])
			{
				vector<int> neighbor_vector = findNeighbor(index_column, frechet_group_mat[index_row - 1]);
				for (int neighbor_flag : neighbor_vector)
				{
					if (frechet_group_mat[index_row - 1][neighbor_flag]>frechet_group_mat[index_row][index_column])
					{
						frechet_group_mat[index_row - 1][neighbor_flag] = frechet_group_mat[index_row][index_column];
					}
					else
					{
						num_change(frechet_group_mat, frechet_group_mat[index_row][index_column], frechet_group_mat[index_row - 1][neighbor_flag]);
					}
				}
				if (frechet_group_mat[index_row][index_column - 1]>frechet_group_mat[index_row][index_column])
				{
					frechet_group_mat[index_row][index_column - 1] = frechet_group_mat[index_row][index_column];
				}
				if (frechet_group_mat[index_row][index_column + 1]>frechet_group_mat[index_row][index_column])
				{
					frechet_group_mat[index_row][index_column + 1] = frechet_group_mat[index_row][index_column];
				}
			}
		}
	}
}


void BwLbel::num_trace(vector<vector<int>>& number_of_group, int pre_num, int tar_num)
{
	for (int i = 0; i < number_of_group[pre_num - 1].size(); i++)
	{
		int num = number_of_group[pre_num - 1][i];
		number_of_group[num - 1].push_back(tar_num);
	}
}

void BwLbel::unique_vector(vector<int>& num_array)
{
	vector<int> new_array;
	int last_num = num_array[0];
	new_array.push_back(last_num);
	for (size_t i = 0; i < num_array.size(); i++)
	{
		if (num_array[i] != last_num)
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

void BwLbel::num_change(vector<vector<int>>& number_of_group, int origin_num, int target_num)
{
	size_t length_row = number_of_group.size();
	size_t length_column = number_of_group[0].size();
	for (int index_row = 0; index_row < length_row; index_row++)
	{
		for (int index_column = 0; index_column < length_column; index_column++)
		{
			if (number_of_group[index_row][index_column] == origin_num)
			{
				number_of_group[index_row][index_column] = target_num;
			}
		}
	}
}