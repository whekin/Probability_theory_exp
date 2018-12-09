#include <iostream>
#include <vector>
#include "ins.h"
#include "experiment.h"
#include "experiment_handler.h"
#include "to_interval.h"
#include "mathAnalytically.h"
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include "table/TextTable.h"
#include "Poligon.h";
#include <cmath>

using namespace std;

int main(int args, char** argv)
{
	int depth = 1;
	int iteration_count = 100;
	bool updated;


	sf::Font roboto;
	roboto.loadFromFile("Roboto-Medium.ttf");

	vector<vector<int>> mathData;

	sf::Color backColor(38, 50, 56);

	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

	sf::RenderWindow window(sf::VideoMode(1200, 800), "Probability theory", sf::Style::Default, settings);
	glLineWidth(2.f);

	vector<int> sumArray;
	vector<vector<int>> data;

	Poligon poligon(window, roboto);

	Poligon math_poligon(window, roboto);
	math_poligon.setMainParams(mathData, 1, 2);
	math_poligon.setBackColor(backColor);
	math_poligon.showLabels = false;
	math_poligon.lineColor = sf::Color::Color(33, 150, 243);
	math_poligon.update();

	sf::Text calculating("Calculating...", roboto);
	calculating.setPosition(window.getSize().x / 2, window.getSize().y / 2);
	calculating.setCharacterSize(25);
	calculating.setOrigin(calculating.getGlobalBounds().width / 2, 0);
	calculating.setFillColor(sf::Color::Color(33, 150, 243));

	sf::Text depth_text("Depth = " + to_string(depth), roboto);
	depth_text.setPosition(10, 10);
	depth_text.setCharacterSize(18);
	sf::Text iteration_count_text("Iter count = " + to_string(iteration_count), roboto);
	iteration_count_text.setCharacterSize(18);
	iteration_count_text.setPosition(10, 35);

	sf::Clock clock;

	sf::Thread thread(
		[
			&mathData,
			&clock,
			&depth_text,
			&iteration_count_text,
			&updated,
			&backColor,
			&poligon,
			&math_poligon,
			&iteration_count,
			&depth,
			&data,
			&sumArray
		]() mutable {

		updated = false;

		cout << "Calculating..." << endl;
		
		clock.restart();
		// if depth didn't change
		mathData = calcDataAnalytically(depth);
		sumArray = doExperiment(depth, iteration_count);
		data = experiment_handler(sumArray);

		system("cls");

		sf::Time calculatingTime = clock.restart();

		cout << "Depth: " << depth << endl;
		cout << "Iteration count: " << iteration_count << endl;
		cout << "Time of calculating: " << calculatingTime.asMilliseconds() << endl;

		vector<double> interval_data;
		vector<vector<int>> idata;
		vector<vector<int>> current_data;

		if (depth > 4)
		{
			poligon.isInterval = true;

			vector<int> d(data.size());
			for (int i = 0; i < data.size(); i++)
				d[i] = data[i][1];

			interval_data = to_interval(d, 20);

			for (int i = 0; i < interval_data.size(); i++)
			{
				vector<int> idata_item(2);
				idata_item[0] = i;
				idata_item[1] = round(interval_data[i]);
				idata.push_back(idata_item);
			}

			current_data = idata;
		}
		else
		{
			if (poligon.isInterval == true)
				poligon.isInterval = false;
			current_data = data;
		}

		float max = 0;
		float min = numeric_limits<float>::max();

		for (auto& num : data)
		{
			if (num[1] > max)
				max = num[1];
			if (num[1] < min)
				min = num[1];
		}

		float mathMax = 0;

		for (auto& num : mathData)
		{
			if (num[1] > mathMax)
				mathMax = num[1];
		}
		
		math_poligon.setMainParams(mathData, 1, mathMax);
		math_poligon.update();

		poligon.setMainParams(current_data, iteration_count, max);
		poligon.setBackColor(backColor);
		poligon.update();
		
		updated = true;

		// console output
		cout << "The maximum: " << round((max * 100.f / iteration_count) * 100.f) / 100 << "%" << " or " << max << endl;
		cout << "The minimum: " << round((min * 100.f / iteration_count) * 100.f) / 100 << "%" << " or " << min << endl;

		TextTable t('-', '|', '+');

		t.add("Sum");
		t.add("Freq %");
		t.add("Math %");
		t.endOfRow();

		for (int i = 0; i < current_data.size(); i++)
		{
			float n = round((current_data[i][1] * 100.f / iteration_count) * 100.f) / 100.f;
			float n2;
			if (depth <= 2)
				n2 = mathData[i][1] * 100.f / calcCountBranches(depth);
			else
				n2 = 0;

			t.add(to_string(current_data[i][0]));
			t.add(toString(n, 2) + "%");
			t.add(toString(n2, 2));
			t.endOfRow();
		}

		cout << t;

		cout << "The interval: " << data.back()[0] - data.front()[0] + 1 << endl;
	});

	thread.launch();

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::Resized:
				window.setView(sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));
				poligon.update();
				math_poligon.update();
				break;
			case sf::Event::KeyPressed:
				switch (event.key.code) {
				case sf::Keyboard::Space:
				case sf::Keyboard::Enter:
					thread.launch();
					break;
				case sf::Keyboard::Up:
					depth++;
					break;
				case sf::Keyboard::Down:
					if (depth > 1)
						depth--;
					break;
				case sf::Keyboard::Right:
					iteration_count += pow(10, digit_of_num(iteration_count) - 1);
					break;

				case sf::Keyboard::Left:
					if (iteration_count > 100) {
						iteration_count -= pow(10, digit_of_num(iteration_count - 1) - 1);
					}
					break;
				}

				depth_text.setString("Depth = " + to_string(depth));
				iteration_count_text.setString("Iter count = " + to_string(iteration_count));

				break;
			}
		}

		calculating.setString("Calculating... (" + to_string(clock.getElapsedTime().asMilliseconds()) + ")");
		calculating.setOrigin(calculating.getGlobalBounds().width / 2, 0);

		window.clear(backColor);
		window.pushGLStates();
		if (depth <= 2)
			math_poligon.render();
		if (updated)
			poligon.render();
		else
			window.draw(calculating);
		window.draw(depth_text);
		window.draw(iteration_count_text);
		window.display();
		window.popGLStates();
	}

	return EXIT_SUCCESS;
}