#include <iostream>
#include <vector>
#include "experiment.h"
#include "experiment_handler.h"
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

// ToDo: следить за определенной суммой и показать график отклонения.

int main(int args, char** argv)
{

	// 2 итерации занимают в 2.5 раза больше времени чем 1.
	int count = 1;
	int iteration_count = 100;
	
	const int windowWidth = 1200;
	const int windowHeight = 800;


	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Probability theory", sf::Style::Default, settings);
	glLineWidth(2.f);

	std::cout << "Calculating..." << std::endl;

	std::vector<int> sumArray;
	std::vector<std::vector<int> > data;

	sumArray = doExperiment(count, iteration_count);
	data = experiment_handler(sumArray);

	sf::VertexArray lines(sf::LinesStrip, data.size());
	
	for (int i = 0; i < data.size(); i++)
	{
		lines[i].position = sf::Vector2f(
			(data[i][0] - data.front()[0]) * windowWidth / (data.back()[0] - data.front()[0]),
			windowHeight - ((data[i][1] * 100.0 / iteration_count) * windowHeight / 100));
	}
	
	sf::Thread thread(
		[&lines, windowWidth, windowHeight, &count, &iteration_count, data, sumArray]() mutable {
		bool repeatIt = true;
		char ans;


		while (repeatIt) {
			sumArray = doExperiment(count, iteration_count);
			data = experiment_handler(sumArray);

			for (auto& num : data)
			{
				std::cout << num[0] << ": " << std::round((num[1] * 100.0 / iteration_count) * 100) / 100 << "%" << std::endl;
			}

			for (int i = 0; i < data.size(); i++)
			{
				lines[i].position = sf::Vector2f(
					(data[i][0] - data.front()[0]) * windowWidth / (data.back()[0] - data.front()[0]),
					windowHeight - ((data[i][1] * 100.0 / iteration_count) * windowHeight / 100));
			}

			std::cout << "The interval: " << data.back()[0] - data.front()[0] + 1 << std::endl;

			std::cout << "calc again? : ";
			std::cin >> ans;


			switch (ans) {
			case 'y':
				repeatIt = true;
				break;
			default:
				repeatIt = false;
			}
		}

	});

	thread.launch();

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type) {
				case sf::Event::Closed:
					window.close();
					break;
			}
		}

		window.clear(sf::Color::Color(38, 50, 56));
		window.pushGLStates();
		window.draw(lines);
		window.display();
		window.popGLStates();
	}

	return 0;
}