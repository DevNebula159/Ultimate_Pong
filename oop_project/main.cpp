#include "./headers/initialize.h"
#include "./headers/global.h"
#include "./headers/utils.h"
#include "./headers/menu.h"
#include "./headers/Court.h"
#include "./headers/Paddle.h"
#include "./headers/Ball.h"
#include "./headers/PongGame.h"



int main() {
	srand(time(0));

	Court court;
	sf::RenderWindow& window = court.getWindow();
	window.setFramerateLimit(60);
	window.setMouseCursorVisible(false);
	window.setMouseCursorGrabbed(true);

	// loading Everyting
	loadFont(font, "fonts/verdana.ttf");
	loadFont(heading, "fonts/scribble.ttf");

	loadingScreen(window);
	loadAllData(window);

	PongGame p;

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
			menuInput(window, event, p);
		}

		//Functions CAll
		gameStateControl(window, event, p);

		window.display();
	}

	return 0;
}
