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

using namespace std;
using namespace sf;

#define v sf::Vector2f
#define c sf::Color

int width = 1920, height = 1080;

// Game States
int gameState = 0;
int check;

// Font
sf::Font font;
sf::Font heading;

//Players
bool player1 = true, player2 = true;
int p1Score = 0, p2Score = 0;

//Delay Clock
Clock inputClock;

//Menu
int menuSelection = 0, total = 2;

int mode = 0;

void setTextinMiddle(sf::Text& text) {
	float x = text.getGlobalBounds().width / 2, y = text.getGlobalBounds().height / 2;
	text.setOrigin(v(x, y));
	text.setPosition(v((float)(width / 2), (float)(height / 2)));
}

class Court {
	sf::RenderWindow window;
public:
	Court() {
		window.create(sf::VideoMode(width, height), "Pong Game", sf::Style::Fullscreen);
	}
	sf::RenderWindow& getWindow() {
		return window;
	}
	void setFrames(float limit) {
		window.setFramerateLimit(limit);
	}
	void close() {
		window.close();
	}
};

class Button {
	sf::Text text;
	sf::Color color;
	float chSize;
public:
	Button() {
		chSize = 70;
	}
	Button(string s, sf::Font& f, float chSize, const c color) {
		this->color = color;
		this->chSize = chSize;
		text.setString(s);
		text.setFillColor(this->color);
		text.setFont(f);
		text.setCharacterSize(this->chSize);
	}

	void operator()(string s, sf::Font& f, float chSize, const c color) {
		this->color = color;
		this->chSize = chSize;
		text.setString(s);
		text.setFillColor(this->color);
		text.setFont(f);
		text.setCharacterSize(this->chSize);
	}

	sf::Text& getText() {
		return text;
	}

	void highlight() {
		text.setFillColor(c::Green);
	}
	void notHighlight() {
		text.setFillColor(this->color);
	}

};


class Menu {
	Button* ptr;
	int n;
public:
	Menu() {
		ptr = nullptr;
		n = 0;
	}

	Menu(Button* other, int n) {
		ptr = other;
		this->n = n;
	}

	void setInMiddle() {
		float temp = (float)(n / 2) * 100;
		if (!(n & 1))
			temp -= 50;
		setTextinMiddle(ptr[0].getText());
		float x = ptr[0].getText().getPosition().x, y = ptr[0].getText().getPosition().y;
		ptr[0].getText().setPosition(v(x, y - temp));
		y = ptr[0].getText().getPosition().y;
		for (int i = 1; i < n; i++) {
			setTextinMiddle(ptr[i].getText());
			ptr[i].getText().setPosition(v(x, y + (i * 100)));
		}
	}

	void display(sf::RenderWindow& window) {
		for (int i = 0;i < n; i++) {
			if (i == menuSelection) {
				ptr[i].highlight();
			}
			else
				ptr[i].notHighlight();
			window.draw(ptr[i].getText());
		}
	}
};


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
		return rec.getPosition().x;
	}
	float getY() {
		return rec.getPosition().y;
	}
	sf::RectangleShape& getBody() {
		return rec;
	}
};

class Paddle : public RecInfo {
	float sensitivity;
public:
	Paddle() : RecInfo(25.0, 135.0, 0, 0) { sensitivity = 0.8; }
	Paddle(float sizeX, float sizeY, float x, float y) : RecInfo(sizeX, sizeY, x, y) { sensitivity = 0.8; }
	Paddle(float x, float y) : RecInfo(20.0, 120.0, x, y) { sensitivity = 0.8; }

	void setToLeft() {
		setPos(sizeX, (height / 2) - (sizeY / 2));
	}

	void setToRight() {
		setPos(width - (2 * sizeX), (height / 2) - (sizeY / 2));
	}

	float getPos() {
		return this->y;
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
public:
	Ball() {
		radius = 15;
		speedX = 0.6, speedY = -0.1;
		circle.setFillColor(c::Blue);
		circle.setRadius(radius);
		circle.setPosition(v(0, 0));
	}
	Ball(float radius, float x, float y) {
		this->radius = radius;
		speedX = 0.6, speedY = -0.1;
		circle.setFillColor(c::Blue);
		circle.setRadius(radius);
		circle.setPosition(v(x, y));
	}

	float getPos() {
		return circle.getPosition().y;
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
		speedX = 0.6, speedY = -0.1;
		float tempY = rand() % 7;
		float tempX = rand() % 2;
		tempY -= 3;
		tempY /= 10;
		if (abs(tempY) - 0 <= 0.01)
			tempY = 0.1;
		speedY = tempY;
		if (!tempX)
			speedX *= -1;

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
			if (speedX < 2) {
				speedX *= 1.15;
				speedY *= 1.2;
			}
			speedX *= -1;
			return true;
		}
		return false;
	}

	bool hitTop() {
		if (circle.getPosition().y <= 0 || circle.getPosition().y + (2 * radius) >= height) {
			speedY *= -1;
			return true;
		}
		return false;
	}

	void gameValid() {
		if (circle.getPosition().x + (2 * radius) < 0) {
			p2Score++;
			player1 = false;
		}
		if (circle.getPosition().x > width + (2 * radius)) {
			p1Score++;
			player2 = false;
		}
	}
};

void drawMenu(sf::RenderWindow& window, int gameState) {
	string s[2] = { "Play", "Exit" }, s1[2] = { "AI", "VS" }, s2[3] = { "Easy", "Medium", "Hard" };
	Button b[2] = { {s[0], font, 70, c::White }, {s[1], font, 70, c::White } };
	Button b1[2] = { {s1[0], font, 70, c::White}, {s1[1], font, 70, c::White} };
	Button b2[3] = { {s2[0], font, 70, c::White}, {s2[1], font, 70, c::White}, {s2[2], font, 70, c::White} };

	if (gameState == 0) {
		Menu m(b, 2);
		m.setInMiddle();
		window.clear();
		m.display(window);
	}
	else if (gameState == 1) {
		Menu m1(b1, 2);
		m1.setInMiddle();
		window.clear();
		m1.display(window);
	}
	else if (gameState == 2) {
		Menu m2(b2, 3);
		m2.setInMiddle();
		window.clear();
		m2.display(window);
	}
}

void mainGame(sf::RenderWindow& window, Paddle& p1, Paddle& p2, Ball& b1, int mode) {
	if (Keyboard::isKeyPressed(Keyboard::W))
		p1.slideUp();
	if (Keyboard::isKeyPressed(Keyboard::S))
		p1.slideDown();
	if (mode == 0)
	{
		if (b1.getPos() > p2.getPos())
		{
			p2.slideDown();
		}
		else if (b1.getPos() < p2.getPos())
			p2.slideUp();
	}
	else if (mode == 1) {
		if (Keyboard::isKeyPressed(Keyboard::Up))
			p2.slideUp();
		if (Keyboard::isKeyPressed(Keyboard::Down))
			p2.slideDown();
	}
	b1.motion();
	b1.hitPaddle(p1);
	b1.hitPaddle(p2);
	b1.hitTop();
	b1.gameValid();
	if (!player1 || !player2) {
		player1 = true;
		player2 = true;
		b1.setInMiddle();
		b1.setSpeed();
	}

	window.clear();
	window.draw(p1.getBody());
	window.draw(p2.getBody());
	window.draw(b1.getBody());
}

void menuInput(sf::RenderWindow& window, sf::Event& event) {
	if (event.type == sf::Event::KeyPressed) {
		if (event.key.code == sf::Keyboard::Escape)
			window.close();
		if (inputClock.getElapsedTime().asSeconds() > 0.2f) {
			if (event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::Up) {
				menuSelection = (menuSelection - 1 + total) % total;
				inputClock.restart();
			}
			else if (event.key.code == sf::Keyboard::S || event.key.code == sf::Keyboard::Down) {
				menuSelection = (menuSelection + 1) % total;
				inputClock.restart();
			}
			else if (event.key.code == sf::Keyboard::Enter || event.key.code == sf::Keyboard::Space) {
				if (gameState == 0) {
					if (menuSelection == 0)
						gameState = 1;
					else
						window.close();
				}
				else if (gameState == 1) {
					if (menuSelection == 0) {
						gameState = 2;
						total = 3;
					}
					else {
						mode = 1;
						gameState = 3;
					}
				}
				else if (gameState == 2) {
					/*if (menuSelection == 0)
					{

					}*/
					gameState = 3;
				}
				inputClock.restart();
			}
		}
	}
}

int main() {
	srand(time(0));

	Court court;
	sf::RenderWindow& window = court.getWindow();

	Paddle p1, p2;
	Ball b1;
	b1.setInMiddle();
	b1.setSpeed();
	p2.setToRight();
	p1.setToLeft();

	if (!font.loadFromFile("fonts/verdana.ttf")) {
		std::cout << "Error loading Verdana Font" << std::endl;
		return 1;
	}
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
			menuInput(window, event);
		}

		//Functions CAll
		if (gameState < 3)
			drawMenu(window, gameState);
		else if (gameState == 3) {
			mainGame(window, p1, p2, b1, mode);
		}

		window.display();
	}

	return 0;
}