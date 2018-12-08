#include "Poligon.h"

Poligon::Poligon(sf::RenderWindow& window, sf::Font& font) : window(window), roboto(font)
{
	lines.setPrimitiveType(sf::LinesStrip);
}

void Poligon::update()
{
	updated = false;
	sf::VertexArray newLines(sf::LinesStrip, data.size());

	adaptiveData.resize(data.size());
	circles.resize(data.size());
	labels.resize(data.size());
	labelRects.resize(data.size());
	bottomMarks.resize(data.size());

	frame.x = window.getSize().x - margin * 2;
	frame.y = window.getSize().y - margin * 2;

	float scaleY = 1.f;

	if (isScaleY)
		scaleY = 100.f / (max * 100.f / iteration_count);

	for (int i = 0; i < data.size(); i++)
	{
		adaptiveData[i] = sf::Vector2f(
			((data[i][0] - data.front()[0]) * frame.x / (data.back()[0] - data.front()[0])) + margin,
			(frame.y - scaleY * ((data[i][1] * 100.f / iteration_count) * frame.y / 100.f) + margin)
			);

		newLines[i].position = adaptiveData[i];
		newLines[i].color = lineColor;

		circles[i].setRadius(circle_radius);
		circles[i].setOrigin(sf::Vector2f(circle_radius, circle_radius));
		circles[i].setPointCount(10);
		circles[i].setPosition(adaptiveData[i]);
		circles[i].setFillColor(lineColor);

		if (showLabels)
		{
			stringstream stream;
			stream << fixed << setprecision(2) << round((data[i][1] * 100.0 / iteration_count) * 100) / 100 << "%";
			string s = stream.str();

			labels[i].setString(s);
			labels[i].setPosition(adaptiveData[i].x - 20, adaptiveData[i].y - 30);
			labels[i].setFont(roboto);
			labels[i].setCharacterSize(17);
			labels[i].setFillColor(textFillColor);

			labelRects[i].setPosition(adaptiveData[i].x - 20, adaptiveData[i].y - 30);
			labelRects[i].setSize(sf::Vector2f(60, 22));
			labelRects[i].setFillColor(backColor);

			if (!isInterval)
			{
				bottomMarks[i].setString(to_string(data[i][0]));
				bottomMarks[i].setPosition(adaptiveData[i].x - 10, frame.y + 50);
				bottomMarks[i].setFont(roboto);
				bottomMarks[i].setCharacterSize(18);
				bottomMarks[i].setFillColor(textFillColor);
			}
		}
	}
	
	lines = newLines;
	updated = true;
}

void Poligon::render()
{
	if (updated) {
		window.draw(lines);

		for (int i = 0; i < data.size(); i++)
		{
			window.draw(circles[i]);
		}

		if (showLabels) {
			for (int i = 0; i < data.size(); i++)
			{
				window.draw(labelRects[i]);
				window.draw(labels[i]);
				if (!isInterval)
					window.draw(bottomMarks[i]);
			}
		}
	}
	
}

void Poligon::setMainParams(vector<vector<int>> &data, int iteration_count, float max)
{
	this->data = data;
	this->iteration_count = iteration_count;
	this->max = max;
}

void Poligon::setBackColor(sf::Color& color)
{
	backColor = color;
}