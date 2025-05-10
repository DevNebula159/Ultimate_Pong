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

int width = 1920, height = 1080;
int gridSize = 30;

int cellX = width / gridSize;
int cellY = height / gridSize;

// Game States
int gameState = 0;
int check;

// Font
sf::Font font;
sf::Font heading;

//Players
bool player1 = true, player2 = true;

void setTextinMiddle(sf::Text& text) {
	float x = text.getGlobalBounds().getSize().x / 2, y = text.getGlobalBounds().getSize().y / 2;
	text.setOrigin(v(x, y));
	text.setPosition(v((float)(width / 2), (float)(height / 2)));
}

// Base Class
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
	float sensitivity;
public:
	Paddle() : RecInfo(20.0, 120.0, 0, 0) { sensitivity = 0.7; }
	Paddle(float sizeX, float sizeY, float x, float y) : RecInfo(sizeX, sizeY, x, y) { sensitivity = 0.7; }
	Paddle(float x, float y) : RecInfo(20.0, 120.0, x, y) { sensitivity =  0.7; }

	void setToLeft() {
		setPos(this->x, (height / 2) - (sizeY / 2));
	}

	void setToRight() {
		setPos(width - sizeX, (height / 2) - (sizeY / 2));
	}

	void setSensitivity(float val) {
		this->sensitivity = val;
	}

	void slideUp() {
		if (this->y > 0) {
			this->y -= sensitivity;
			rec.move(v(0, (-1) * sensitivity));
		}
	}
	void slideDown() {
		if (this->y + sizeY < (float)height) {
			this->y += sensitivity;
			rec.move(v(0, sensitivity));
		}
	}
};

class Ball {
	sf::CircleShape circle;
	float radius;
	float speedX, speedY;
	int hitCount;
public:
	Ball() {
		radius = 10;
		speedX = 0.3, speedY = -0.1;
		circle.setFillColor(c::Blue);
		circle.setRadius(radius);
		circle.setPosition(v(0, 0));
		hitCount = 0;
	}
	Ball(float radius, float x, float y) {
		this->radius = radius;
		speedX = 0.35, speedY = -0.1;
		circle.setFillColor(c::Blue);
		circle.setRadius(radius);
		circle.setPosition(v(x, y));
		hitCount = 0;
	}
	void setPs(float x, float y) {
		circle.setPosition(v(x, y));
	}
	void setColor(c val) {
		circle.setFillColor(val);
	}
	void setColor(int r, int g, int b) {
		circle.setFillColor(c(r, g, b));
	}
	void setSpeed() {
		float temp = rand() % 2;
		if (!temp) {
			speedX *= -1;
			speedY *= -1;
		}
	}
	void setInMiddle() {
		float halfX = (width / 2) - (radius / 2);
		float halfY = (height / 2) - (radius / 2);
		setPs(halfX, halfY);
	}
	sf::CircleShape& getBody() {
		return circle;
	}

	void motion() {
		circle.move(v(speedX, speedY));
	}

	bool hitPaddle(Paddle& other) {
		if (circle.getGlobalBounds().intersects(other.getBody().getGlobalBounds())) {
			hitCount++;
			if (hitCount & 1)
				speedY *= 1.1;
			else
				speedY *= 0.92;
			if (speedX < 3)
				speedX *= 1.05;
			speedX *= -1;
			return true;
		}
		return false;
	}

	bool hitTop() {
		if (circle.getPosition().y <= 0 || circle.getPosition().y + radius >= height) {
			speedY *= -1;
			return true;
		}
		return false;
	}

	void gameValid() {
		if (circle.getPosition().x + (2 * radius) < 0)
			player1 = false;
		if (circle.getPosition().x > width + (2 * radius))
			player2 = false;
		if (!player1 || !player2)
			gameState = 1;
	}
};

void mainGame(sf::RenderWindow& window, Paddle& p1, Paddle& p2, Ball& b1) {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		p1.slideUp();
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		p1.slideDown();
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		p2.slideUp();
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		p2.slideDown();
	b1.motion();
	b1.hitPaddle(p1);
	b1.hitPaddle(p2);
	b1.hitTop();
	b1.gameValid();


	window.clear();
	window.draw(p1.getBody());
	window.draw(p2.getBody());
	window.draw(b1.getBody());
}

void gameOver(sf::RenderWindow& window) {
	sf::Text t1("GameOver", font, 50), announce("Player 1 Lost", font, 30);

	setTextinMiddle(t1);
	t1.setPosition(v(t1.getPosition().x, t1.getPosition().y - 200));
	t1.setFillColor(c::White);
	
	if (!player2)
		announce.setString("Player 2 Lost");
	setTextinMiddle(announce);
	announce.setFillColor(c::White);

	window.clear();
	window.draw(t1);
	window.draw(announce);
}

int main() {
	srand(time(0));

	sf::RenderWindow window(sf::VideoMode(width, height), "Pong Game", sf::Style::Fullscreen);
	Paddle p1, p2;
	Ball b1;
	b1.setInMiddle();
	b1.setSpeed();
	p1.setToLeft();
	p2.setToRight();

	if (!font.loadFromFile("fonts/verdana.ttf")) {
		std::cout << "Error loading Verdana Font" << std::endl;
		return 1;
	}
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Escape)
					window.close();
			}

		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			window.close();
		//Functions CAll
		if (gameState == 0)
			mainGame(window, p1, p2, b1);
		if (gameState == 1)
			gameOver(window);

		window.display();
	}

	return 0;
}