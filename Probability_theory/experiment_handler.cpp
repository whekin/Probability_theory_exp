#include <vector>
#include <algorithm>
#include "experiment.h"

std::vector<std::vector<int> > experiment_handler(int iteration_count)
{
	std::vector<int> arr;
	std::vector<std::vector<int> > res;

	int count = 1;

	arr = doExperiment(iteration_count);
	std::sort(arr.begin(), arr.end());
	
	for (int i = 1; i < iteration_count; i++)
	{
		if (arr[i] == arr[i - 1]) {
			count++;
			
		}
		else {
			std::vector<int> res_item;
			res_item.push_back(arr[i - 1]);
			res_item.push_back(count);
			res.push_back(res_item);
			count = 1;
		}
	}

	std::vector<int> res_item;
	res_item.push_back(arr[iteration_count - 1]);
	res_item.push_back(count);
	res.push_back(res_item);

	return res;
}