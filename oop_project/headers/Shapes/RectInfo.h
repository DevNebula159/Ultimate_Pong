#include "../initialize.h"
#include "../global.h"

#pragma once
// Base Class of a Rectangle
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
	void setColor() {
		rec.setFillColor(c(pR, pG, pB));
		pG -= 25;
		pB -= 25;
	}
	float getX() {
		return rec.getPosition().x;
	}
	float getY() {
		return rec.getPosition().y;
	}

	virtual void setOriginalLength() {
		this->sizeY = 170.0;
		rec.setSize(v(sizeX, sizeY));
	}

	sf::RectangleShape& getBody() {
		return rec;
	}
};