#include "SFML/Audio.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Network.hpp"
#include "SFML/System.hpp"
#include "SFML/Window.hpp"

#include <iostream>
#include <fstream>
#include <Windows.h>
#include <vector>
#include <algorithm>
#include <cmath>
#include <string>
#include <stack>
#include <cstdlib>
#include <ctime>
#include <time.h>

#define v sf::Vector2f
#define c sf::Color

int width = 1280, height = 720;
int gridSize = 30;

int cellX = width / gridSize;
int cellY = height / gridSize;

// Game States
int gameState = -1;
int check;

// Font
sf::Font font;
sf::Font heading;

class RecInfo {
protected:
	sf::RectangleShape rec;
	float x, y, sizeX, sizeY;
public:
	RecInfo() {
		sizeX = 30, sizeY = 30;
		this->x = 0.0;
		this->y = 0.0;
		rec.setSize(v(sizeX, sizeY));
		rec.setPosition(v(x, y));
		rec.setFillColor(c::White);
	}
	RecInfo(float sizeX, float sizeY, float x, float y) {
		this->sizeX = sizeX;
		this->sizeY = sizeY;
		this->x = x;
		this->y = y;
		rec.setSize(v(this->sizeX, this->sizeY));
		rec.setPosition(v(this->x, this->y));
		rec.setFillColor(c::White);
	}
	void setSz(float x, float y) {
		this->sizeX = x;
		this->sizeY = y;
		rec.setSize(v(this->sizeX, this->sizeY));
	}
	void setPos(float posX, float posY) {
		this->x = posX;
		this->y = posY;
		rec.setPosition(v(x, y));
	}
	void setColor(c val) {
		rec.setFillColor(val);
	}
	void setColor(int r, int g, int b) {
		rec.setFillColor(c(r, g, b));
	}
	float getX() {
		return this->x;
	}
	float getY() {
		return this->y;
	}
	sf::RectangleShape& getBody() {
		return rec;
	}
};

class Paddle: public RecInfo {
public:
	Paddle(): RecInfo(20.0, 120.0, 0, 0) {}
	Paddle(float sizeX, float sizeY, float x, float y): RecInfo(sizeX, sizeY, x, y){}
	Paddle(float x, float y): RecInfo(20.0, 120.0, x, y){}

	void setToRight() {
		setPos(width - sizeX, this->y);
	}

	void slideUp() {
		if (this->y > 0) {
			this->y -= 10;
			rec.move(v(0, -10));
		}
	}
	void slideDown() {
		if (this->y < (float)height) {
			this->y += 10;
			rec.move(v(0, 10));
		}
	}
};

class Ball {
	sf::CircleShape circle;
	float radius, x, y;
public:
	Ball() {
		radius = 10, x = 0, y = 0;
		circle.setFillColor(c::Blue);
		circle.setRadius(radius);
		circle.setPosition(x, y);
	}
	Ball(float radius, float x, float y) {
		this->radius = radius;
		this->x = x;
		this->y = y;
		circle.setFillColor(c::Blue);
		circle.setRadius(radius);
		circle.setPosition(x, y);
	}
	void setPos(float x, float y) {
		this->x = x;
		this->y = y;
		circle.setPosition(v(x, y));
	}
	void setColor(c val) {
		circle.setFillColor(val);
	}
	void setColor(int r, int g, int b) {
		circle.setFillColor(c(r, g, b));
	}
	void setInMiddle() {
		//float halfX = (radiusX / 2) - (radius / 2);
		//float halfY = (radiusY / 2) - (radius / 2);
		//setPos(halfX, halfY);
		circle.setOrigin(v(radius, radius));
		setPos((float)width / 2, (float)height / 2);
	}
	sf::CircleShape& getBody() {
		return circle;
	}
};


int main() {
	sf::RenderWindow window(sf::VideoMode(width, height), "Pong Game", sf::Style::Default);
	sf::CircleShape circle(50.0f);
	circle.setFillColor(sf::Color::Blue);
	Paddle p1, p2;
	Ball b1;
	b1.setInMiddle();
	p2.setToRight();

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::W)
				p1.slideUp();
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::S)
				p1.slideDown();	
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Up)
				p2.slideUp();
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Down)
				p2.slideDown();
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
				window.close();

		}


		window.clear();
		window.draw(p1.getBody());
		window.draw(p2.getBody());
		window.draw(b1.getBody());

		window.display();
	}

	return 0;
}