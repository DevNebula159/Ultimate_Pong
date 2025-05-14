#pragma once
#include "./Shapes/Circle.h"
#include "./Paddle.h"
#include "./utils.h"
#include "./utils2.h"

void vibrateController(int controllerIndex, WORD leftMotor, WORD rightMotor) {
	XINPUT_VIBRATION vibration = {};
	vibration.wLeftMotorSpeed = leftMotor;
	vibration.wRightMotorSpeed = rightMotor;
	XInputSetState(controllerIndex, &vibration);
}

// and you also know that it is a baaaaaaaalll
class Ball : public circleInfo {
	float speedX, speedY;
	CircleShape circle;
	float radius;
public:
	Ball() {
		speedX = 20, speedY = -10;
		radius = 15;
		circle.setFillColor(c::Blue);
		circle.setRadius(radius);
	}
	Ball(float radius, float x, float y) {
		speedX = 20, speedY = -10;
		this->radius = radius;
		circle.setFillColor(c::Blue);
		circle.setRadius(radius);
		circle.setPosition(v(x, y));
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

	void setInMiddle() {
		float halfX = (width / 2) - (radius / 2);
		float halfY = (height / 2) - (radius / 2);
		setPs(halfX, halfY);
	}

	sf::CircleShape& getBody() {
		return circle;
	}

	void display(sf::RenderWindow& window) {
		window.draw(circle);
	}

	void motion() {
		circle.move(v(speedX, speedY));
	}

	bool checkPaddle(Paddle& other) {
		if (circle.getGlobalBounds().intersects(other.getBody().getGlobalBounds()))
			return true;
		else
			return false;
	}

	bool hitPaddle(Paddle& other) {
		if (circle.getGlobalBounds().intersects(other.getBody().getGlobalBounds())) {
			if (pG > 0)
				other.setColor();
			startShakeEffect();
			vibrateController(0, 65535, 65535);
			hitPaddleSound.setPosition(Vector3f(other.getX(), other.getY(), 0));
			hitPaddleSound.play();
			if (speedX < 2) {
				speedX *= 1.15;
				speedY *= 1.2;
			}
			speedX *= -1;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W) || checkAxisMoved(0, 1, 0) || checkAxisMoved(1, 1, 0)) {
				if (speedY > 0) speedY *= -1;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S) || checkAxisMoved(0, 0, 1) || checkAxisMoved(1, 0, 1)) {
				if (speedY < 0) speedY *= -1;
			}
			return true;
		}
		return false;
	}

	bool hitTop() {
		if (circle.getPosition().y <= 9.7f || circle.getPosition().y + (2 * radius) >= height - 9.7f) {
			speedY *= -1;
			return true;
		}
		return false;
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

