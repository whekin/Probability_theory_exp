#pragma once
#include <vector>
#include "experiment_handler.h"
#include "combinations.h"

using namespace std;


vector<vector<int>> calcDataAnalytically(int depth) // 1 or 2
{
	vector<int> universe;
	for (int i = 0; i < 4; i++)
		universe.push_back(i + 1);

	vector<vector<int>> combinations;
	combinations = calcCombinations(universe, 2);

	int universe_sum = 0;
	for (int i = 0; i < universe.size(); i++)
	{
		universe_sum += universe[i];
	}

	vector<int> sum_array;
	vector<int> out_array;

	for (int i = 0; i < combinations.size(); i++)
	{
		int sum = combinations[i][0] + combinations[i][1];
		sum += universe_sum;

		sum_array.push_back(sum);
	}

	out_array = sum_array;

	if (depth == 2)
	{
		out_array.clear();
		int len = sum_array.size();

		for (int i = 0; i < len; i++)
		{
			int cur_sum = sum_array[i];
			vector<int> cur_universe = universe;
			cur_universe.push_back(cur_sum - universe_sum);

			vector<vector<int>> cur_combinations;
			cur_combinations = calcCombinations(cur_universe, 2);

			for (int i = 0; i < cur_combinations.size(); i++)
			{
				int sum = cur_combinations[i][0] + cur_combinations[i][1];
				sum += cur_sum;
				out_array.push_back(sum);
			}

			cur_universe.clear();
		}
	}

	vector<vector<int>> data = experiment_handler(out_array);

	return data;
}
