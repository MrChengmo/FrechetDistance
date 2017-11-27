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
}

FrechetDistance3D::~FrechetDistance3D()
{
}

double FrechetDistance3D::frechetDistance(vector<vector<double>> &P1, vector<vector<double>> &P2)
{
	vector<double> x1, x2, y1, y2, z1, z2;
	double result = -1,ratio;
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
	vector<vector<int>> frechet_compare_matrix;
	for (double range = f_min; range < f_max; range += res)
	{	
		int mode = 0;//bwlabel = 0, astar = 1;
		compareMatrix(frechet_dis_matrix, frechet_compare_matrix, range,mode);
		//Astar astar;
		//astar.InitAstar(frechet_compare_matrix);
		//Point start(0, 0);
		//Point end(frechet_compare_matrix.size() - 1, frechet_compare_matrix[0].size() - 1);
		//list<Point *> path = astar.GetPath(start, end, true);
		//if (!path.empty())
		//{
		//	result = range;
		//	break;
		//}
		BwLbel bwlabel_process;
		vector<vector<int>> frechet_group_mat = bwlabel_process.bwlabel(frechet_compare_matrix);
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
	ratio = getRatio(result, f_max, f_min);
	return ratio;
}

void FrechetDistance3D::compareMatrix(vector<vector<double>>& origin_matrix, vector<vector<int>>& result_matrix, double & range,int mode)
{
	vector<int> frechet_compare_row;
	for (size_t index_row = 0; index_row < origin_matrix.size(); index_row++)
	{
		for (size_t index_column = 0; index_column < origin_matrix[index_row].size(); index_column++)
		{
			if (origin_matrix[index_row][index_column]<=range)
			{
				if (mode == 0)
				{
					frechet_compare_row.push_back(1);
				}
				else if(mode == 1)
				{
					frechet_compare_row.push_back(0);
				}			
			}
			else
			{
				if (mode == 0)
				{
					frechet_compare_row.push_back(0);
				}
				else if (mode == 1)
				{
					frechet_compare_row.push_back(1);
				}
			}
		}
		result_matrix.push_back(frechet_compare_row);
		frechet_compare_row.clear();
	}
}

double FrechetDistance3D::getRatio(double result, double f_max, double f_min)
{
	double probablity = (1-normcdf((result-f_min)/(f_max/3)))*2;
	return probablity;
}









