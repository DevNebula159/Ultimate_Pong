#pragma once
#include "utils.h"
#include "utils2.h"
#include "initialize.h"
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

// A lot of Buttons together make a menu
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