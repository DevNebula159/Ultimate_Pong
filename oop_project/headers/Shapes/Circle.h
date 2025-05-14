#include "../initialize.h"
#pragma once
// Base Class of a circle
class circleInfo {
public:
	virtual float getX() = 0;

	virtual float getY() = 0;

	virtual void setPs(float x, float y) = 0;

	virtual void setColor(c val) = 0;

	virtual void setColor(int r, int g, int b) = 0;

	virtual void setInMiddle() = 0;

	virtual CircleShape& getBody() = 0;

	virtual void display(sf::RenderWindow& window) = 0;
};
