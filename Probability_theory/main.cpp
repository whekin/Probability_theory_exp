#include <iostream>
#include <iomanip>
#include <vector>
#include "experiment.h"
#include "experiment_handler.h"
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

int main(int args, char** argv)
{
	int count = 1;
	int iteration_count = 10000;
	bool isScaleY = true;

	const int windowWidth = 1200;
	const int windowHeight = 800;


	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Probability theory", sf::Style::Default, settings);
	glLineWidth(2.f);

	std::vector<int> sumArray;
	std::vector<std::vector<int> > data;

	sf::VertexArray lines(sf::LinesStrip, 0);
	
	sf::Thread thread(
		[&lines, windowWidth, windowHeight, count, iteration_count, isScaleY, &data, &sumArray] () mutable {
		bool repeatIt = true;
		char ans;

		while (repeatIt) {
			std::cout << "Calculating..." << std::endl;
			sf::Clock clock;

			sumArray = doExperiment(count, iteration_count);
			data = experiment_handler(sumArray);

			sf::Time calculatingTime = clock.restart();

			std::cout << "Time of calculating: " << calculatingTime.asMilliseconds() << std::endl;

			float max = 0;
			float min = std::numeric_limits<float>::max();

			for (auto& num : data)
			{
				if (num[1] > max)
					max = num[1];
				if (num[1] < min)
					min = num[1];
			}

			std::cout << "The maximum: " << std::round((max * 100.f / iteration_count) * 100.f) / 100 << "%" << std::endl;
			std::cout << "The minimum: " << std::round((min * 100.f / iteration_count) * 100.f) / 100 << "%" << std::endl;

			const int maxCount = 5;
			const int width = 12;

			int row, column;
			int counter = 0;

			for (row = 1; counter < data.size(); row++)
			{
				for (column = 1; column <= maxCount && counter < data.size(); column++)
				{
					std::cout << std::setw(width) << data[counter][0] << ": " << std::round((data[counter][1] * 100.0 / iteration_count) * 100) / 100 << "%";
					counter++;
				}
				std::cout << std::endl;
			}

			
			sf::VertexArray newLines(sf::LinesStrip, data.size());

			float scaleY = 1.f;
			if (isScaleY)
				scaleY = 100.f / (max * 100.f / iteration_count);

			for (int i = 0; i < data.size(); i++)
			{
				newLines[i].position = sf::Vector2f(
					((data[i][0] - data.front()[0]) * windowWidth / (data.back()[0] - data.front()[0])),
					(windowHeight - scaleY * ((data[i][1] * 100.f / iteration_count) * windowHeight / 100.f)));
			}
			
			lines = newLines;

			std::cout << "The interval: " << data.back()[0] - data.front()[0] + 1 << std::endl;

			std::cout << "Does it calc again? : ";
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