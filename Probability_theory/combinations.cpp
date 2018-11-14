#include <vector>

std::vector<std::vector<int> > calcCombinations(std::vector<int> universe, int sampleSize)
{
	std::vector<std::vector<int> > combinationsSet;
	
	for (int i = 0; i < universe.size() - 1; i++)
	{
		for (int j = i; j <= universe.size() - sampleSize; j++)
		{
			std::vector<int> combination;
			for (int k = 0; k < sampleSize - 1; k++)
				combination.push_back(universe[i + k]);
			combination.push_back(universe[j + sampleSize - 1]);
			combinationsSet.push_back(combination);
		}
	}
	return combinationsSet;
}