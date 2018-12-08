#include <vector>
#include <algorithm>

using namespace std;

vector<vector<int> > experiment_handler(vector<int> sumArray)
{
	vector<vector<int> > res;

	int count = 1;

	sort(sumArray.begin(), sumArray.end());
	
	for (int i = 1; i < sumArray.size(); i++)
	{
		if (sumArray[i] == sumArray[i - 1])
			count++;
		else
		{
			vector<int> res_item;
			res_item.push_back(sumArray[i - 1]);
			res_item.push_back(count);
			res.push_back(res_item);
			count = 1;
		}
	}

	vector<int> res_item;
	res_item.push_back(sumArray[sumArray.size() - 1]);
	res_item.push_back(count);
	res.push_back(res_item);

	return res;
}