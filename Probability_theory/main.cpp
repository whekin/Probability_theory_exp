#include <iostream>
#include <iomanip>
#include <vector>
#include "experiment.h"
#include "experiment_handler.h"
#include "to_interval.h"
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include "table/TextTable.h"
#include "Poligon.h";
#include <sstream>
#include <cmath>

using namespace std;

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

int main(int args, char** argv)
{
	int depth = 1;
	int iteration_count = 100;
	bool updated;

	sf::Font roboto;
	roboto.loadFromFile("Roboto-Medium.ttf");

	int mathPoints1Static[][2] = {
		{ 13, 1 },
		{ 14, 1 },
		{ 15, 2 },
		{ 16, 1 },
		{ 17, 1 }
	};

	vector<vector<int>> mathPoints1(5);
	for (int i = 0; i < 5; i++)
	{
		vector<int> el(2);
		el[0] = mathPoints1Static[i][0];
		el[1] = mathPoints1Static[i][1];

		mathPoints1[i] = el;
	}

	sf::Color backColor(38, 50, 56);

	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

	sf::RenderWindow window(sf::VideoMode(1200, 800), "Probability theory", sf::Style::Default, settings);
	glLineWidth(2.f);

	vector<int> sumArray;
	vector<vector<int>> data;

	Poligon poligon(window, roboto);

	Poligon mathPoligon1(window, roboto);
	mathPoligon1.setMainParams(mathPoints1, 6, 2);
	mathPoligon1.setBackColor(backColor);
	mathPoligon1.showLabels = false;
	mathPoligon1.lineColor = sf::Color::Color(33, 150, 243);
	mathPoligon1.update();

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

	/*Poligon mathPoligon2(window);
	mathPoligon2.setMainParams(mathPoints1, 13, 2);
	mathPoligon2.setBackColor(backColor);
	mathPoligon2.showLabels = false;
	mathPoligon2.lineColor = sf::Color::Color(100, 100, 100);
	mathPoligon2.update();*/


	sf::Clock clock;

	sf::Thread thread(
		[
			&clock,
			&depth_text,
			&iteration_count_text,
			&updated,
			&backColor,
			&poligon,
			&mathPoligon1,
			&iteration_count,
			&depth,
			&data,
			&sumArray
		]() mutable {

		updated = false;

		cout << "Calculating..." << endl;
		
		clock.restart();

		sumArray = doExperiment(depth, iteration_count);
		data = experiment_handler(sumArray);

		system("cls");

		sf::Time calculatingTime = clock.restart();

		cout << "Depth: " << depth << endl;
		cout << "Iteration count: " << iteration_count << endl;
		cout << "Time of calculating: " << calculatingTime.asMilliseconds() << endl;

		float max = 0;
		float min = numeric_limits<float>::max();

		for (auto& num : data)
		{
			if (num[1] > max)
				max = num[1];
			if (num[1] < min)
				min = num[1];
		}

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
		t.endOfRow();

		for (auto& num : current_data)
		{
			float n = round((num[1] * 100.0 / iteration_count) * 100) / 100;

			stringstream stream;
			stream << fixed << setprecision(2) << n;
			string s = stream.str();

			t.add(to_string(num[0]));
			t.add(s + "%");
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
				mathPoligon1.update();
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
		if (depth == 1)
			mathPoligon1.render();
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