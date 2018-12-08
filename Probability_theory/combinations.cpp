#include <vector>

using namespace std;

vector<vector<int> > calcCombinations(vector<int> universe, int sampleSize)
{
	vector<vector<int> > combinationsSet;
	
	for (int i = 0; i < universe.size() - 1; i++)
		for (int j = i; j <= universe.size() - sampleSize; j++)
		{
			vector<int> combination;
			for (int k = 0; k < sampleSize - 1; k++)
				combination.push_back(universe[i + k]);
			combination.push_back(universe[j + sampleSize - 1]);
			combinationsSet.push_back(combination);
		}
	return combinationsSet;
}