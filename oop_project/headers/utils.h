#pragma once
#include "./menu.h"
#include "./PongGame.h"
#include "./initialize.h"



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

void lastHigh() {
	vector<string> highN;
	vector<int> highS;
	string name;
	int score;
	int size = 0;
	ifstream fin("lastHigh.txt");
	while (fin >> name >> score) {
		highN.push_back(name);
		highS.push_back(score);
		size++;
	}
	bool valid = 1;
	int ind;
	fin.close();
	for (int i = 0; i < size && valid; i++)
	{
		if (username == highN[i])
		{
			ind = i;
			valid = 0;
		}
	}
	if (!valid)
	{
		if (lead > highS[ind])
		{
			highS[ind] = lead;
			ofstream fout("lastHigh.txt");
			for (int i = 0; i < size; i++)
			{
				fout << highN[i] << " " << highS[i] << endl;
			}
			fout.close();
		}
	}
	else {
		ofstream fout("lastHigh.txt", ios::app);
		fout << username << " " << lead << endl;
		fout.close();
	}
}

void history() {
	ofstream fout("history.txt", ios::app);
	fout << username << " " << lead << endl;
	fout.close();
}

void darj() {
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
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
			darj();
			history();
			lastHigh();
			loadHighScores();
			inputClock.restart();
			gameState = 5;
		}
		else if (event.type == sf::Event::TextEntered) {
			if (event.text.unicode == 8) { // Backspace
				if (username != "")
					username.erase(username.size() - 1, 1);
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

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) || isJoystickButtonPressed(0, 1) || isJoystickButtonPressed(1, 1) || isJoystickButtonPressed(0, 6) || isJoystickButtonPressed(1, 6)) {
		escapeSound.play();
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

// it makes and draws multiple menus in their respective gamestates.
void drawMenu(sf::RenderWindow& window, int gameState) {
	string s[3] = { "Play", "High Scores","Exit" }, s1[2] = { "AI", "VS" }, s2[3] = { "Easy", "Medium", "Hard" }, s3[3] = { "Resume", "Return to Main Menu", "Restart" };
	string s4[3] = { "Restart", "Return to Main Menu", "Exit" };
	Button b[3] = { {s[0], font, 50, c::White }, {s[1], font, 50, c::White }, {s[2], font, 50, c::White} };
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
		setHeading(overText);

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

// it is used to create a delay in the program
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

void initializeGame(PongGame& p) {
	pR = 255, pG = 255, pB = 255;
	p.p1.setColor(c::White);
	p.p2.setColor(c::White);
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
	p.setListener();
	p.setMode(mode);
	p.checks();
	p.startGame();
	p.render(window);
	if (delayCount < 2) // 2 is perfectly fine, 1 is too short
		delayCount++;
	if (delayCount == 2) {
		checkTime();
		delayCount++;
	}
}

// it handles the input and consequences of pressing innocent buttons in the menus
void menuInput(sf::RenderWindow& window, sf::Event& event, PongGame& p) {
	if (event.type == sf::Event::KeyPressed || sf::Joystick::isButtonPressed || event.type == sf::Event::JoystickMoved) {

		if (inputClock.getElapsedTime().asSeconds() > 0.2f) {
			if (event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::Up || checkAxisMoved(0, 1, 0) || checkAxisMoved(1, 1, 0)) {
				menuSelection = (menuSelection - 1 + total) % total;
				scrollSound.play();
				inputClock.restart();
			}
			else if (event.key.code == sf::Keyboard::S || event.key.code == sf::Keyboard::Down || checkAxisMoved(0, 0, 1) || checkAxisMoved(1, 0, 1)) {
				menuSelection = (menuSelection + 1) % total;
				scrollSound.play();
				inputClock.restart();
			}
			else if (event.key.code == sf::Keyboard::Escape || isJoystickButtonPressed(0, 6) || isJoystickButtonPressed(1, 6) || isJoystickButtonPressed(0, 1) || isJoystickButtonPressed(1, 1)) {
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
				if (gameState != 3 && gameState != -2)
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
void loadAllData(RenderWindow& window) {
	view = window.getDefaultView();
	view.zoom(0.99f);
	viewCenter = view.getCenter();

	//Load HighScores
	loadHighScores();

	// Loading of Sounds
	loadSound(scrollBuffer, scrollSound, "sounds/scroll.wav", 1);
	scrollSound.setPitch(1.5f);
	loadSound(selectBuffer, selectSound, "sounds/select.mp3", 0);
	selectSound.setPitch(2.0f);
	loadSound(escapekBuffer, escapeSound, "sounds/escape.mp3", 1);
	escapeSound.setVolume(30.0f);
	escapeSound.setPitch(1.4f);
	loadSound(hitPaddleBuffer, hitPaddleSound, "sounds/paddle_sound.mp3", 0);
	hitPaddleSound.setPitch(2.0f);
	hitPaddleSound.setRelativeToListener(true);
	hitPaddleSound.setAttenuation(60.0f);
	hitPaddleSound.setMinDistance(10.0f);
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
}

void loadingScreen(RenderWindow& window) {
	sf::Text loadingText("Loading...", font, 50);
	setTextinMiddle(loadingText);
	window.clear();
	window.draw(loadingText);
	window.display();
	sf::sleep(sf::milliseconds(50));
}

void gameStateControl(RenderWindow& window, Event& event, PongGame& p) {
	if (gameState == 3) {
		window.setView(view);
	}
	else
		window.setView(window.getDefaultView());
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
}
