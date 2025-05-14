#include "./Shapes/RectInfo.h"
#pragma once
// Paddle - you know it is a paddle. it is the name of class 
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
		if (this->y > 9.7f) {
			this->y -= sensitivity;
			rec.move(v(0, (-1) * sensitivity));
		}
	}
	void slideDown() {
		if (this->y + sizeY < (float)height - 9.7f) {
			this->y += sensitivity;
			rec.move(v(0, sensitivity));
		}
	}
};
