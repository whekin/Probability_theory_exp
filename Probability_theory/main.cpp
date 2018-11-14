#include <iostream>
#include <vector>
#include "experiment_handler.h"

int main(int args, char** argv) {
	int iteration_count = 100000;

	std::vector<std::vector<int> > sumArray;
	sumArray = experiment_handler(iteration_count);

	for (auto& num : sumArray) {
		std::cout << num[0] << ": " << std::round((num[1] * 100.0 / iteration_count) * 100) / 100 << "%" << std::endl;
	}

	system("pause");
	return 0;
}