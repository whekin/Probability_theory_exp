#pragma once
#include <sstream>
#include <iomanip>
#include <string>

using namespace std;

string toString(float num, int n)
{
	stringstream stream;
	stream << fixed << setprecision(n) << num;
	return stream.str();
}

int fact(int num)
{
	if (num == 1)
		return 1;
	else
		return num * fact(num - 1);
}

int calcCountBranches(int n) {
	int count = 1;

	for (int i = 1; i <= n; i++)
	{
		count *= fact(i + 3) / (fact(2) * fact(i + 1));
	}

	return count;
}

int digit_of_num(int num)
{
	int c = 0;
	int digit = num;

	while (digit > 0)
	{
		digit /= 10;
		c++;
	}

	return c;
}