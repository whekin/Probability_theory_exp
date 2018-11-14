#include <vector>
#include <random>
#include "combinations.h";

std::vector<int> doExperiment(int iterations_count)
{
	const int sample_size = 2;
	const int universe_length = 4;

	std::vector<int> universe(universe_length);
	std::vector<int> sumArray;
	std::vector<std::vector<int> > combinationSet;
	
	std::random_device rd;
	std::mt19937 gen(rd());

	for (int i = 0; i < universe_length; i++)
	{
		universe.at(i) = i + 1;
	}

	combinationSet = calcCombinations(universe, sample_size);

	for (int i = 0; i < iterations_count; i++)
	{
		int sum = 0;
		int number_randomly_combination = gen() % combinationSet.size();

		for (int j = 0; j < universe_length; j++)
			sum += universe[j];

		for (int j = 0; j < sample_size; j++)
			sum += combinationSet[number_randomly_combination][j];

		sumArray.push_back(sum);
	}

	return sumArray;
}