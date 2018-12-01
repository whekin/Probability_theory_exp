#include <vector>
#include <algorithm>
#include "experiment.h"

std::vector<std::vector<int> > experiment_handler(std::vector<int> sumArray)
{
	std::vector<std::vector<int> > res;

	int count = 1;

	std::sort(sumArray.begin(), sumArray.end());
	
	for (int i = 1; i < sumArray.size(); i++)
	{
		if (sumArray[i] == sumArray[i - 1])
			count++;
		else
		{
			std::vector<int> res_item;
			res_item.push_back(sumArray[i - 1]);
			res_item.push_back(count);
			res.push_back(res_item);
			count = 1;
		}
	}

	std::vector<int> res_item;
	res_item.push_back(sumArray[sumArray.size() - 1]);
	res_item.push_back(count);
	res.push_back(res_item);

	return res;
}