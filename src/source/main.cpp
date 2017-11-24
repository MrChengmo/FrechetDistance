#include "FrechetDistance.h"
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#define PATH "E:/Project/cm_project/FrechetDistance/testcase"
using namespace std;
double test();
void readline(string line_string, vector<vector<double>> &data);

int main()
{	
	return test();
}

void readline(string line_string, vector<vector<double>> &data)
{ 
	int index_begin = 0, index_end = 0,length =0;
	vector<double> row_data;
	for (index_end; index_end < line_string.size(); index_end++)
	{
		if (line_string[index_end] == ',' || line_string[index_end] == '\t')
		{
			row_data.push_back(stof(line_string.substr(index_begin, length)));
			index_begin = index_end + 1;
			length = 0;
		}
		/*else if (index_end == line_string.size() - 1)
		{
			row_data.push_back(stof(line_string.substr(index_begin, length + 1)));
		}*/
		else
		{
			length++;
		}
	}
	data.push_back(row_data);
}

double test()
{
	
	string true_result_path = string(PATH) + "/true_result.csv";
	string test_result_path = string(PATH) + "/location_call_back.csv";
	vector<vector<double>> true_result;
	vector<vector<double>> test_result;
	std::fstream true_result_read;
	true_result_read.open(true_result_path, std::ios::in);
	string line_string;
	while (getline(true_result_read, line_string))
	{
		readline(line_string, true_result);
	}
	std::fstream test_result_read;
	test_result_read.open(test_result_path, std::ios::in);
	while (getline(test_result_read,line_string))
	{
		readline(line_string, test_result);
	}
	FrechetDistance3D frechetDistance;
	double result = frechetDistance.frechetDistance(true_result, test_result);
	return result;
}