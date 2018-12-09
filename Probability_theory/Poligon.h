#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>

using namespace std;

class Poligon
{
private:
	
	sf::RenderWindow& window;
	sf::VertexArray lines;
	sf::Font roboto;

	vector<sf::Vector2f> adaptiveData;
	vector<sf::CircleShape> circles;
	vector<sf::Text> labels;
	vector<sf::RectangleShape> labelRects;
	vector<sf::Text> bottomMarks;

	bool isScaleY = true;

	struct Frame {
		float x;
		float y;
	} frame;

	float margin = 50;

	bool updated = false;

	float circle_radius = 5.f;

public:
	Poligon(sf::RenderWindow&, sf::Font&);

	bool showLabels = true;
	bool isInterval = false;
	float max;
	int iteration_count;
	vector<vector<int>> data;

	sf::Color textFillColor = sf::Color::Color(33, 150, 243);
	sf::Color backColor;
	sf::Color lineColor = sf::Color::White;

	void update();

	void render();

	void setMainParams(vector<vector<int>> &data, int iteration_count, float max);

	void setBackColor(sf::Color& color);
};

