#pragma once
#include <vector>

using namespace std;

vector<double> to_interval(vector<int>& data, int dev_count) {
	vector<double> res(dev_count);

	int h = data.size() / dev_count;

	for (int i = 0; i < dev_count; i++)
	{
		double average = 0;
		for (int j = 0; j < h; j++)
		{
			average += data[i*h + j];
		}

		average /= h;
		res[i] = average;
	}
	
	return res;
}
	