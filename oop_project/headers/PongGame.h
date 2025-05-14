#pragma once
#include "./Ball.h"
#include "./Paddle.h"
#include "./utils.h"
#include "./utils2.h"
// This class handles the functionality of the gameState in which the game is being played
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
		endShakeEffect();
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) || isJoystickButtonPressed(0, 7) || isJoystickButtonPressed(1, 7)) {
			this->saveData();
			total = 3;
			gameState = 4;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || checkAxisMoved(0, 1, 0))
			p1.slideUp();
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || checkAxisMoved(0, 0, 1))
			p1.slideDown();
		if (mode <= 2) {
			if (mode == 0)
				p2.setSensitivity(9);
			if (mode == 1)
				p2.setSensitivity(12);
			if (mode == 2)
				p2.setSensitivity(15);

			if (b.getY() >= p2.getY() + p2.getSensitivity())
				p2.slideDown();
			else if (b.getY() < p2.getY() - p2.getSensitivity())
				p2.slideUp();
		}
		else if (mode == 3) {
			p2.setSensitivity(20);
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || checkAxisMoved(1, 1, 0))
				p2.slideUp();
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || checkAxisMoved(1, 0, 1))
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
				pR = 255, pG = 255, pB = 255;
				p1.setColor(c::White);
				p2.setColor(c::White);
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
			menuSelection = 0;
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
		b.hitPaddle(p2);
		if (b.hitPaddle(p1)) {
			inputClock.restart();
		}
		if (inputClock.getElapsedTime().asSeconds() > 0.5 || b.checkPaddle(p2)) {
			vibrateController(0, 0, 0);
		}
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
	void setListener() {
		sf::Listener::setPosition(sf::Vector3f(b.getX(), b.getY(), 0));
	}
	friend void initializeGame(PongGame& p);
};
