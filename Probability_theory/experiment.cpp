#include <vector>
#include <random>
#include <iostream>
#include "combinations.h";

std::vector<int> doExperiment(int count, int iterations_count)
{
	int sample_size = 2;
	int universe_length = 4;

	std::vector<int> universe;
	std::vector<int> sumArray;
	std::vector<std::vector<int> > combinationSet;
	
	std::random_device rd;
	std::mt19937 gen(rd());

	for (int i = 0; i < iterations_count; i++)
	{
		universe.clear();
		for (int j = 0; j < universe_length; j++)
			universe.push_back(j + 1);

		for (int j = 0; j < count - 1; j++)
		{
			combinationSet = calcCombinations(universe, sample_size);
			int number_randomly_combination = gen() % combinationSet.size();
			int combinationSum = 0;

			for (int k = 0; k < sample_size; k++)
				combinationSum += combinationSet[number_randomly_combination][k];

			universe.push_back(combinationSum);
		}
		
		combinationSet = calcCombinations(universe, sample_size);

		int sum = 0;
		int number_randomly_combination = gen() % combinationSet.size();

		int combinationSum = 0;
	
		for (int j = 0; j < sample_size; j++)
			combinationSum += combinationSet[number_randomly_combination][j];

		for (int j = 0; j < universe.size(); j++)
			sum += universe[j];
		
		for (int j = 0; j < sample_size; j++)
			sum += combinationSet[number_randomly_combination][j];

		sumArray.push_back(sum);
	}

	return sumArray;
}