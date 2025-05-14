#pragma once
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
