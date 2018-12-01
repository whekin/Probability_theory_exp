#include <vector>
#include <random>

std::vector<int> doExperiment(int count, int iterations_count)
{
	int sample_size = 2;
	int universe_length = 4;

	std::vector<int> universe;
	std::vector<int> defaultUniverse;
	std::vector<int> sumArray;
	std::vector<std::vector<int> > combinationSet;
	
	std::random_device rd;
	std::mt19937 gen(rd());

	for (int i = 0; i < universe_length; i++)
		defaultUniverse.push_back(i + 1);

	for (int i = 0; i < iterations_count; i++)
	{
		universe = defaultUniverse;

		for (int j = 0; j < count; j++)
		{
			int combinationSum = 0;
			int firstRandom = gen() % universe.size();
			int secondRandom = gen() % universe.size();

			while (firstRandom == secondRandom)
				secondRandom = gen() % universe.size();

			combinationSum += universe[firstRandom] + universe[secondRandom];

			universe.push_back(combinationSum);
		}
		
		int sum = 0;
		
		for (int j = 0; j < universe.size(); j++)
			sum += universe[j];
		
		sumArray.push_back(sum);
	}

	return sumArray;
}