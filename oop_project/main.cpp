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

#define v sf::Vector2f
#define c sf::Color

int width = 1920, height = 1080;

// Game States
int gameState = -3;
int check;

// Font
sf::Font font;
sf::Font heading;

// Heading Color
float hR = 43, hG = 110, hB = 176, hSize = 100;

//Players
bool player1 = true, player2 = true;
int p1Score = 0, p2Score = 0;

// Storage of Data during Pause
float p1tempX = 0, p1tempY = 0, p2tempX = 0, p2tempY = 0, balltempX = 0, balltempY = 0, ballspeedX = 0, ballspeedY = 0;
int p1scoretemp = 0, p2scoretemp = 0;

// MaxScore
int maxScore = 10;

//Delay Clock
sf::Clock inputClock;

//Menu
int menuSelection = 0, total = 3;

// Textures
sf::Texture texture, texture1, gbgTexture;

//Sprites
sf::Sprite sprite, sprite1, gbgSprite;

// Sounds
sf::SoundBuffer scrollBuffer, selectBuffer, escapekBuffer, hitPaddleBuffer, winBuffer, bgmBuffer;
sf::Sound scrollSound, selectSound, escapeSound, hitPaddleSound, winSound;
sf::Music bgm;

//Intro Items
sf::Texture intro1T, intro2T, intro3T, intro4T, intro5T, intro6T, intro7T, intro8T, intro9T, intro10T, intro11T;
sf::Sprite intro1S;
sf::SoundBuffer sbIntro;
sf::Sound sIntro;

// Name Input
//Username
string username = "";
sf::Text userInputText;

//HighScore
vector<string> names;
vector<int> scores;
int lead = 0;

int mode = 0;

// sets the text to the middle of screen
void setTextinMiddle(sf::Text& text) {
	float x = text.getGlobalBounds().width / 2, y = text.getGlobalBounds().height / 2;
	text.setOrigin(v(x, y));
	text.setPosition(v((float)(width / 2), (float)(height / 2)));
}

// Check if a JoyStick Button is Pressed
bool isJoystickButtonPressed(unsigned int joystickId, unsigned int button) {
	return sf::Joystick::isConnected(joystickId) && sf::Joystick::isButtonPressed(joystickId, button);
}

// Check if a JoyStick Axis is Movedd
bool checkAxisMoved(unsigned int id, float x, float y, bool Up, bool Down) {
	if (!sf::Joystick::isConnected(id)) {
		return false;
	}
	if (x == 101 || y == 101) {
		return false;
	}
	float axisMoved = y, axisNotMoved = x;
	if (abs(x) >= abs(y)) {
		return false;
	}

	if (std::abs(x) > 15 || std::abs(y) > 15) {
		if (abs(axisMoved) - 100 <= 0.1 && abs(axisNotMoved) < 5) {
			if (Up && axisMoved < 99.0f)
				return true;
			if (Down && axisMoved > 99.0f)
				return true;
		}
	}
	return false;
}

void setHeading(sf::Text& text) {
	text.setCharacterSize(100);
	text.setFillColor(c(hR, hG, hB));
	setTextinMiddle(text);
	text.setPosition(v(text.getPosition().x, text.getPosition().y - 300));
}

void loadHighScores() {
	ifstream fin("./Highscores.txt");
	int score;
	string name;
	for (int i = 0; i < 5; i++)
	{
		fin >> name;
		names.push_back(name);
		fin >> score;
		scores.push_back(score);
	}
	fin.close();
}

void darj(int lead, string username) {
	for (int i = 0; i < 5; i++)
	{
		int temp;
		string tName;
		if (scores[i] < lead)
		{
			for (int j = i; j < 4; j++)
			{
				if (j == i)
				{
					temp = scores[j + 1];
					tName = names[j + 1];
					swap(scores[j], scores[j + 1]);
					swap(names[j], names[j + 1]);
				}
				else {
					swap(temp, scores[j + 1]);
					swap(tName, names[j + 1]);
				}
			}
			scores[i] = lead;
			names[i] = username;
			break;
		}
	}
	ofstream fout("./Highscores.txt");
	for (int i = 0; i < 5; i++)
	{
		cout << names[i] << " " << scores[i] << endl;
		fout << names[i] << " " << scores[i] << endl;
	}
	fout.close();
	username = "";
}

void getUsername(sf::Event& event, sf::RenderWindow& window, string& username) {
	sf::Text askName("Enter Your Name", font, 100);
	setHeading(askName);

	sf::Text win("Player 1 Won", font, 60);
	win.setFillColor(c(0, 10, 10));
	win.setLetterSpacing(0.8);
	setTextinMiddle(win);
	win.setPosition(v(win.getPosition().x, win.getPosition().y + 300));

	if (p2scoretemp == maxScore)
		win.setString("Player 2 Won");

	userInputText.setFont(font);
	userInputText.setCharacterSize(40);
	userInputText.setFillColor(c::White);
	setTextinMiddle(userInputText);

	if (inputClock.getElapsedTime().asSeconds() > 0.12f) {
		if (event.type == sf::Event::TextEntered) {
			if (event.text.unicode == 8) { // Backspace
				if (username != "")
					username.erase(username.size() - 1, 1);
			}
			else if (event.text.unicode == 13) { // Enter
				darj(lead, username);
				loadHighScores();
				gameState = 5;
			}
			else if (event.text.unicode < 128) { // Valid ASCII
				username += (char)event.text.unicode;
			}
			userInputText.setString(username);
			inputClock.restart();
		}
	}

	// Draw
	window.clear();
	window.draw(sprite);
	window.draw(userInputText);
	window.draw(askName);
	window.draw(win);
}

void intro(sf::RenderWindow& window) {

	static sf::Clock clock;
	float elapsedTime = clock.getElapsedTime().asSeconds() + 0.5;

	if (elapsedTime <= 0.62)
		intro1S.setTexture(intro4T);
	else if (elapsedTime <= 0.7)
		intro1S.setTexture(intro5T);
	else if (elapsedTime <= 0.78)
		intro1S.setTexture(intro6T);
	else if (elapsedTime <= 0.86)
		intro1S.setTexture(intro7T);
	else if (elapsedTime <= 0.94)
		intro1S.setTexture(intro8T);
	else if (elapsedTime <= 1.02)
		intro1S.setTexture(intro9T);
	else if (elapsedTime <= 1.1)
		intro1S.setTexture(intro10T);
	else if (elapsedTime <= 1.18)
		intro1S.setTexture(intro11T);
	else if (elapsedTime <= 1.26)
		intro1S.setTexture(intro10T);
	else if (elapsedTime <= 1.34)
		intro1S.setTexture(intro9T);
	else if (elapsedTime <= 1.42)
		intro1S.setTexture(intro8T);
	else if (elapsedTime <= 1.5)
		intro1S.setTexture(intro7T);
	else if (elapsedTime <= 1.58)
		intro1S.setTexture(intro8T);
	else if (elapsedTime <= 1.66)
		intro1S.setTexture(intro9T);
	else if (elapsedTime <= 1.74)
		intro1S.setTexture(intro10T);
	else if (elapsedTime <= 1.82)
		intro1S.setTexture(intro11T);
	else if (elapsedTime <= 1.9)
		intro1S.setTexture(intro10T);
	else if (elapsedTime <= 1.98)
		intro1S.setTexture(intro9T);
	else if (elapsedTime <= 2.06)
		intro1S.setTexture(intro8T);
	else if (elapsedTime <= 2.14)
		intro1S.setTexture(intro7T);
	else if (elapsedTime <= 2.22)
		intro1S.setTexture(intro8T);
	else if (elapsedTime <= 3) {
		if (elapsedTime > 2.5)
			sIntro.stop();
		intro1S.setTexture(intro9T);
	}
	else {
		bgm.play();
		gameState = 0;
	}

	// Draw
	window.clear();
	window.draw(intro1S);
}

void setScoreDetails(sf::Text& text, string data) {
	text.setFont(font);
	text.setString(data);
	text.setCharacterSize(50);
	text.setFillColor(c::White);
	setTextinMiddle(text);
}

void displayHighScores(sf::RenderWindow& window) {
	sf::Text scoreHeading("HighScores", font, 100), goBack("Press Escape to Return to Main Menu", font, 40);
	goBack.setFillColor(c(40, 40, 40));
	setTextinMiddle(goBack);
	goBack.setPosition(v(goBack.getPosition().x, goBack.getPosition().y + 400));
	setHeading(scoreHeading);
	scoreHeading.setPosition(v(scoreHeading.getPosition().x, scoreHeading.getPosition().y - 100));

	const int n = 5;
	sf::Text list[n], list1[n];
	for (int i = 0; i < n; i++) {
		string data = names[i];
		setScoreDetails(list[i], data);
		data = to_string(scores[i]);
		setScoreDetails(list1[i], data);
	}

	float tempPos = (float)(n / 2) * 100;;
	float x = list[0].getPosition().x, y = list[0].getPosition().y;
	float x1 = list1[0].getPosition().x, y1 = list1[0].getPosition().y;
	list[0].setPosition(v(x - 150, y - tempPos));
	list1[0].setPosition(v(x1 + 150, y1 - tempPos));
	y = list[0].getPosition().y;
	y1 = list1[0].getPosition().y;
	for (int i = 1; i < n; i++) {
		list[i].setPosition(v(x - 150, y + (i * 100)));
		list1[i].setPosition(v(x1 + 150, y1 + (i * 100)));
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
		gameState = 0;
		total = 3;
	}

	//Draw
	window.clear();
	window.draw(sprite);
	window.draw(scoreHeading);
	window.draw(goBack);
	for (int i = 0; i < n; i++) {
		window.draw(list[i]);
		window.draw(list1[i]);
	}
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
		text.setFillColor(c(26, 102, 171));
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
	Paddle() : RecInfo(25.0, 200.0, 0, 0) { sensitivity = 20; }
	Paddle(float sizeX, float sizeY, float x, float y) : RecInfo(sizeX, sizeY, x, y) { sensitivity = 20; }
	Paddle(float x, float y) : RecInfo(25.0, 200.0, x, y) { sensitivity = 20; }

	void setToLeft() {
		setPos(sizeX, (height / 2) - (sizeY / 2));
	}

	void setToRight() {
		setPos(width - (2 * sizeX), (height / 2) - (sizeY / 2));
	}

	float getPos() {
		return this->y;
	}

	float getSizeX() {
		return sizeX;
	}

	float getSizeY() {
		return sizeY;
	}

	void setLength(float val) {
		this->sizeY = val;
		rec.setSize(v(sizeX, sizeY));
	}

	void setSensitivity(float val) {
		this->sensitivity = val;
	}

	float getSensitivity() {
		return this->sensitivity;
	}

	void setOriginalLength() {
		this->sizeY = 170.0;
		rec.setSize(v(sizeX, sizeY));
	}

	void aiUp(float val) {
		if (this->y > 0) {
			this->y -= val;
			rec.move(v(0, (-1) * val));
		}
	}
	void aiDown(float val) {
		if (this->y + sizeY < (float)height) {
			this->y += val;
			rec.move(v(0, val));
		}
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
		speedX = 20, speedY = -10;
		circle.setFillColor(c::Blue);
		circle.setRadius(radius);
		circle.setPosition(v(0, 0));
	}
	Ball(float radius, float x, float y) {
		this->radius = radius;
		speedX = 20, speedY = -10;
		circle.setFillColor(c::Blue);
		circle.setRadius(radius);
		circle.setPosition(v(x, y));
	}

	float getPos() {
		return circle.getPosition().y;
	}

	float getX() {
		return circle.getPosition().x;
	}

	float getY() {
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

	void setSpeed(float x, float y) {
		speedX = x, speedY = y;
		this->motion();
	}

	void setSpeed() {
		speedX = 15, speedY = -10;
		float tempY = rand() % 20;
		float tempX = rand() % 2;
		tempY -= 10;
		if (abs(tempY) - 0 <= 1)
			tempY = 10;
		speedY = tempY;
		if (!tempX)
			speedX *= -1;

	}

	float getSpeedX() {
		return speedX;
	}
	float getSpeedY() {
		return speedY;
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
		float stickX = 101, stickY = 101, stick1X = 101, stick1Y = 101;
		if (sf::Joystick::isConnected(0)) {
			stickX = sf::Joystick::getAxisPosition(0, sf::Joystick::X);
			stickY = sf::Joystick::getAxisPosition(0, sf::Joystick::Y);
		}
		if (sf::Joystick::isConnected(1)) {
			stick1X = sf::Joystick::getAxisPosition(1, sf::Joystick::X);
			stick1Y = sf::Joystick::getAxisPosition(1, sf::Joystick::Y);
		}
		if (circle.getGlobalBounds().intersects(other.getBody().getGlobalBounds())) {
			hitPaddleSound.play();
			if (speedX < 2) {
				speedX *= 1.15;
				speedY *= 1.2;
			}
			speedX *= -1;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W) || checkAxisMoved(0, stickX, stickY, 1, 0) || checkAxisMoved(1, stickX, stickY, 1, 0)) {
				if (speedY > 0) speedY *= -1;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S) || checkAxisMoved(0, stickX, stickY, 0, 1) || checkAxisMoved(1, stickX, stickY, 0, 1)) {
				if (speedY < 0) speedY *= -1;
			}
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

	void display(sf::RenderWindow& window) {
		window.draw(circle);
	}

	void gameValid() {
		if (circle.getPosition().x + (2 * radius) < 0) {
			player1 = false;
		}
		if (circle.getPosition().x > width + (2 * radius)) {
			player2 = false;
		}
	}
};

void drawMenu(sf::RenderWindow& window, int gameState) {
	string s[3] = { "Play", "High Scores","Exit"}, s1[2] = {"AI", "VS"}, s2[3] = {"Easy", "Medium", "Hard"}, s3[3] = {"Resume", "Return to Main Menu", "Restart"};
	string s4[3] = { "Restart", "Return to Main Menu", "Exit" };
	Button b[3] = { {s[0], font, 50, c::White }, {s[1], font, 50, c::White }, {s[2], font, 50, c::White}};
	Button b1[2] = { {s1[0], font, 50, c::White}, {s1[1], font, 50, c::White} };
	Button b2[3] = { {s2[0], font, 50, c::White}, {s2[1], font, 50, c::White}, {s2[2], font, 50, c::White} };

	Button b3[3] = { {s3[0], font, 50, c::White}, {s3[1], font, 50, c::White}, {s3[2], font, 50, c::White} };
	Button b4[3] = { {s4[0], font, 50, c::White}, { s4[1], font, 50, c::White }, {s4[2], font, 50, c::White} };

	if (gameState == 0) {
		sf::Text title("Pong Game", heading, 130);
		title.setLetterSpacing(1.9f);
		title.setFillColor(c(hR, hG, hB));
		setTextinMiddle(title);
		title.setPosition(v(title.getPosition().x, title.getPosition().y - 350));
		Menu m(b, 3);
		m.setInMiddle();

		// Draw
		window.clear();
		window.draw(sprite);
		m.display(window);
		window.draw(title);
	}
	else if (gameState == 1) {
		sf::Text selectMode("Select Mode", font, hSize);
		setHeading(selectMode);
		Menu m1(b1, 2);
		m1.setInMiddle();

		//Draw
		window.clear();
		window.draw(sprite);
		m1.display(window);
		window.draw(selectMode);
	}
	else if (gameState == 2) {
		sf::Text selectDifficulty("Select Difficulty", font, hSize);
		setHeading(selectDifficulty);
		Menu m2(b2, 3);
		m2.setInMiddle();

		//Draw
		window.clear();
		window.draw(sprite);
		m2.display(window);
		window.draw(selectDifficulty);
	}
	else if (gameState == 4) {
		Menu m3(b3, 3);
		m3.setInMiddle();
		window.clear();
		window.draw(sprite);
		m3.display(window);
	}
	else if (gameState == 5) {
		sf::Text overText("GameOver", font, 80);
		overText.setFillColor(c(0, 128, 255));
		setTextinMiddle(overText);
		overText.setPosition(v(overText.getPosition().x, overText.getPosition().y - 300));

		sf::Text win("Player 1 Won", font, 60);
		win.setFillColor(c(0, 10, 10));
		win.setLetterSpacing(0.8);
		setTextinMiddle(win);
		win.setPosition(v(win.getPosition().x, win.getPosition().y + 300));

		if (p2scoretemp == maxScore)
			win.setString("Player 2 Won");

		Menu m4(b4, 3);
		m4.setInMiddle();

		//Draw
		window.clear();
		window.draw(sprite);
		window.draw(overText);
		window.draw(win);
		m4.display(window);
	}
}

bool checkTime() {
	sf::Clock clk;
	float elapsed = 0;
	while (elapsed <= 0.5f) {
		elapsed = clk.getElapsedTime().asSeconds();
		if (elapsed >= 0.5f)
			return true;
	}
	return false;
}
int delayCount = 0;
class PongGame {
	int p1Score, p2Score;
	Ball b;
	Paddle p1, p2;
	int mode;
public:
	PongGame() {
		p1Score = 0, p2Score = 0;
		b.setInMiddle();
		p2.setToRight();
		p1.setToLeft();
		p1.setSensitivity(20);
		p2.setSensitivity(20);
		mode = 0;
	}
	void setMode(int mode) {
		this->mode = mode;
	}
	void checks() {
		float stickX = 101, stickY = 101, stick1X = 101, stick1Y = 101;
		if (sf::Joystick::isConnected(0)) {
			stickX = sf::Joystick::getAxisPosition(0, sf::Joystick::X);
			stickY = sf::Joystick::getAxisPosition(0, sf::Joystick::Y);
		}
		if (sf::Joystick::isConnected(1)) {
			stick1X = sf::Joystick::getAxisPosition(1, sf::Joystick::X);
			stick1Y = sf::Joystick::getAxisPosition(1, sf::Joystick::Y);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) || isJoystickButtonPressed(0, 7) || isJoystickButtonPressed(1, 7)) {
			this->saveData();
			total = 3;
			gameState = 4;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || checkAxisMoved(0, stickX, stickY, 1, 0))
			p1.slideUp();
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || checkAxisMoved(0, stickX, stickY, 0, 1))
			p1.slideDown();
		if (mode <= 2) {
			if (mode == 0)
				p2.setSensitivity(9);
			if (mode == 1)
				p2.setSensitivity(12);
			if (mode == 2)
				p2.setSensitivity(15);

			if (b.getPos() >= p2.getPos() + p2.getSensitivity())
				p2.slideDown();
			else if (b.getPos() < p2.getPos() - p2.getSensitivity())
				p2.slideUp();
		}
		else if (mode == 3) {
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || checkAxisMoved(1, stick1X, stick1Y, 1, 0))
				p2.slideUp();
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || checkAxisMoved(1, stick1X, stick1Y, 0, 1))
				p2.slideDown();
		}
		if (!player1 && p2Score < maxScore) {
			p2Score++;
			p2.setLength(p2.getSizeY() - 7.0f);
		}
		if (!player2 && p1Score < maxScore) {
			p1Score++;
			p1.setLength(p1.getSizeY() - 7.0f);
		}
		if (!player1 || !player2) {
			if (p1Score < maxScore && p2Score < maxScore) {
				player1 = true;
				player2 = true;
				p1.setToLeft();
				p2.setToRight();
				b.setInMiddle();
				delayCount = 0;
				b.setSpeed();
			}
		}
		if (p1Score == maxScore)
			p1scoretemp = maxScore;
		if (p2Score == maxScore)
			p2scoretemp = maxScore;
		if (p1Score == maxScore || p2Score == maxScore) {
			player1 = true, player2 = true;
			total = 3;
			check = 3;
			lead = abs(p1Score - p2Score);
			if (mode == 3) {
				username = "";
				userInputText.setString(username);
				gameState = -1;
			}
			else
				gameState = 5;
			winSound.play();
		}
	}
	void startGame() {
		b.motion();
		b.hitPaddle(p1);
		b.hitPaddle(p2);
		b.hitTop();
		b.gameValid();
	}

	void saveData() {
		p1tempX = p1.getX(), p1tempY = p1.getY();
		p2tempX = p2.getX(), p2tempY = p2.getY();
		balltempX = b.getX(), balltempY = b.getY();
		p1scoretemp = p1Score, p2scoretemp = p2Score;
		ballspeedX = b.getSpeedX(), ballspeedY = b.getSpeedY();
		b.setSpeed(0, 0);
	}

	void loadData(float temp1X, float temp1Y, float temp2X, float temp2Y, float bX, float bY, float speedX, float speedY, int tempScore1, int tempScore2) {
		p1.setPos(temp1X, temp1Y);
		p2.setPos(temp2X, temp2Y);
		b.setPs(bX, bY);
		b.setSpeed(speedX, speedY);
		p1Score = tempScore1;
		p2Score = tempScore2;
	}

	void render(sf::RenderWindow& window) {
		window.clear();
		window.draw(gbgSprite);
		string score1 = to_string(p1Score);
		sf::Text s1(score1, font, 70);
		string score2 = to_string(p2Score);
		sf::Text s2(score2, font, 70);
		s1.setFillColor(c::White);
		s2.setFillColor(c::White);
		s1.setPosition(v(500, 50));
		s2.setPosition(v(1420, 50));
		window.draw(s1);
		window.draw(s2);
		window.draw(b.getBody());
		window.draw(p1.getBody());
		window.draw(p2.getBody());
	}
	friend void initializeGame(PongGame& p);
};

void initializeGame(PongGame& p) {
	p.p1.setOriginalLength();
	p.p2.setOriginalLength();
	p.p1.setToLeft();
	p.p2.setToRight();
	p.b.setInMiddle();
	p.b.setSpeed();
	p.p1Score = 0;
	p.p2Score = 0;
	player1 = true;
	player2 = true;
}

void mainGame(sf::RenderWindow& window, int mode, PongGame& p) {
	p.setMode(mode);
	p.checks();
	p.startGame();
	p.render(window);
	if (delayCount < 2)
		delayCount++;
	if (delayCount == 2) {
		checkTime();
		delayCount++;
	}
}

void menuInput(sf::RenderWindow& window, sf::Event& event, PongGame& p) {
	float stickX = 101, stickY = 101, stick1X = 101, stick1Y = 101;
	if (sf::Joystick::isConnected(0)) {
		stickX = sf::Joystick::getAxisPosition(0, sf::Joystick::X);
		stickY = sf::Joystick::getAxisPosition(0, sf::Joystick::Y);
	}
	if (sf::Joystick::isConnected(1)) {
		stick1X = sf::Joystick::getAxisPosition(1, sf::Joystick::X);
		stick1Y = sf::Joystick::getAxisPosition(1, sf::Joystick::Y);
	}
	if (event.type == sf::Event::KeyPressed || sf::Joystick::isButtonPressed || event.type == sf::Event::JoystickMoved) {

		if (inputClock.getElapsedTime().asSeconds() > 0.2f) {
			if (event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::Up || checkAxisMoved(0, stickX, stickY, 1, 0) || checkAxisMoved(1, stick1X, stick1Y, 1, 0)) {
				menuSelection = (menuSelection - 1 + total) % total;
				scrollSound.play();
				inputClock.restart();
			}
			else if (event.key.code == sf::Keyboard::S || event.key.code == sf::Keyboard::Down || checkAxisMoved(0, stickX, stickY, 0, 1) || checkAxisMoved(1, stick1X, stick1Y, 0, 1)) {
				menuSelection = (menuSelection + 1) % total;
				scrollSound.play();
				inputClock.restart();
			}
			else if (event.key.code == sf::Keyboard::Escape || isJoystickButtonPressed(0, 6) || isJoystickButtonPressed(1, 6)) {
				escapeSound.play();
				if (gameState == 0) {
					window.close();
				}
				else if (gameState == 1) {
					gameState = 0;
					total = 3;
				}
				else if (gameState == 2) {
					gameState = 1;
					total = 2;
				}
				else if (gameState == 4) {
					p.loadData(p1tempX, p1tempY, p2tempX, p2tempY, balltempX, balltempY, ballspeedX, ballspeedY, p1scoretemp, p2scoretemp);
					gameState = 3;
				}
				menuSelection = 0;
				inputClock.restart();
			}
			else if (event.key.code == sf::Keyboard::Enter || event.key.code == sf::Keyboard::Space || isJoystickButtonPressed(0, 0) || isJoystickButtonPressed(1, 0)) {
				if (gameState != 3  && gameState != -2)
					selectSound.play();
				if (gameState == 0) {
					if (menuSelection == 0) {
						total = 2;
						gameState = 1;
					}
					else if (menuSelection == 1)
						gameState = -2;
					else
						window.close();
				}
				else if (gameState == 1) {
					if (menuSelection == 0) {
						gameState = 2;
						total = 3;
					}
					else {
						mode = 3;
						initializeGame(p);
						gameState = 3;
					}
				}
				else if (gameState == 2) {
					if (menuSelection == 0)
						mode = 0;
					if (menuSelection == 1)
						mode = 1;
					if (menuSelection == 2)
						mode = 2;
					menuSelection = 0;
					initializeGame(p);
					delayCount = 0;
					gameState = 3;
				}
				else if (gameState == 4) {
					if (menuSelection == 0) {
						p.loadData(p1tempX, p1tempY, p2tempX, p2tempY, balltempX, balltempY, ballspeedX, ballspeedY, p1scoretemp, p2scoretemp);
						gameState = 3;
					}
					else if (menuSelection == 1) {
						total = 3;
						gameState = 0;
					}
					else {
						initializeGame(p);
						delayCount = 0;
						gameState = 3;
					}
				}
				else if (gameState == 5) {
					winSound.stop();
					if (menuSelection == 0) {
						initializeGame(p);
						delayCount = 0;
						gameState = 3;
					}
					else if (menuSelection == 1) {
						initializeGame(p);
						total = 3;
						gameState = 0;
					}
					else
						window.close();
				}
				menuSelection = 0;
				inputClock.restart();
			}
		}
	}
}

void loadSound(sf::SoundBuffer& buffer, sf::Sound& sound, string path, bool loud) {
	if (!buffer.loadFromFile(path)) {
		cout << "Error loading Sound" << endl;
	}
	sound.setBuffer(buffer);
	if (loud)
		hitPaddleSound.setVolume(20.0f);
}

void loadFont(sf::Font& font, string path) {
	font.loadFromFile(path);
}

void loadSprite(sf::Texture& texturetemp, sf::Sprite& spritetemp, string path) {
	texturetemp.loadFromFile(path);
	spritetemp.setTexture(texturetemp);
}

int main() {
	srand(time(0));

	Court court;
	sf::RenderWindow& window = court.getWindow();
	window.setFramerateLimit(60);
	window.setMouseCursorVisible(false);
	window.setMouseCursorGrabbed(true);

	//Load HighScores
	loadHighScores();

	// loading of fonts
	loadFont(font, "fonts/verdana.ttf");
	loadFont(heading, "fonts/scribble.ttf");

	sf::Text loadingText("Loading...", font, 50);
	setTextinMiddle(loadingText);
	window.clear();
	window.draw(loadingText);
	window.display();

	PongGame p;

	// Loading of Sounds
	loadSound(scrollBuffer, scrollSound, "sounds/scroll.wav", 1);
	scrollSound.setPitch(1.5f);
	loadSound(selectBuffer, selectSound, "sounds/select.mp3", 0);
	selectSound.setPitch(2.0f);
	loadSound(escapekBuffer, escapeSound, "sounds/escape.mp3", 1);
	escapeSound.setVolume(30.0f);
	escapeSound.setPitch(1.4);
	loadSound(hitPaddleBuffer, hitPaddleSound, "sounds/hitPaddle1.wav", 0);
	hitPaddleSound.setPitch(2.0f);
	loadSound(winBuffer, winSound, "sounds/victory.mp3", 1);
	bgm.openFromFile("sounds/bgm.mp3");
	bgm.setLoop(1);

	// Loading of Textures
	loadSprite(texture, sprite, "bg/title.jpg");
	loadSprite(gbgTexture, gbgSprite, "bg/gbg1.png");
	// Load Intro Screen Images
	intro1T.loadFromFile("bg/intro1.png");
	intro2T.loadFromFile("bg/intro2.png");
	intro3T.loadFromFile("bg/intro3.png");
	intro4T.loadFromFile("bg/intro4.png");
	intro5T.loadFromFile("bg/intro5.png");
	intro6T.loadFromFile("bg/intro6.png");
	intro7T.loadFromFile("bg/intro7.png");
	intro8T.loadFromFile("bg/intro8.png");
	intro9T.loadFromFile("bg/intro9.png");
	intro10T.loadFromFile("bg/intro10.png");
	intro11T.loadFromFile("bg/intro11.png");

	// Load Intro Sound
	if (!sbIntro.loadFromFile("sounds/iSound.wav"))
		cout << "Failed to load Intro Sound!" << endl;
	sIntro.setBuffer(sbIntro);

	sIntro.play();

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
			menuInput(window, event, p);
		}

		//Functions CAll
		if (gameState == -3)
			intro(window);
		else if (gameState == -1) {
			getUsername(event, window, username);
		}
		else if (gameState == -2)
			displayHighScores(window);
		else if (gameState == 0 || gameState == 1 || gameState == 2 || gameState == 4 || gameState == 5)
			drawMenu(window, gameState);
		else if (gameState == 3) {
			mainGame(window, mode, p);
		}

		if (gameState == 3 || gameState == -1 || gameState == -2)
			scrollSound.setVolume(0.0f);
		else
			scrollSound.setVolume(12.0f);
		if (gameState >= 3) {
			bgm.setVolume(0.0f);
		}
		else
			bgm.setVolume(20.0f);

		window.display();
	}

	return 0;
}