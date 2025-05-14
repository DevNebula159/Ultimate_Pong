#include "./headers/global.h"

int width = 1920, height = 1080;
int gameState = -3;
int check;

Font font;
Font heading;

float hR = 43, hG = 110, hB = 176, hSize = 100;

int pR = 255, pG = 255, pB = 255;


bool player1 = true, player2 = true;
int p1Score = 0, p2Score = 0;

float p1tempX = 0, p1tempY = 0, p2tempX = 0, p2tempY = 0, balltempX = 0, balltempY = 0, ballspeedX = 0, ballspeedY = 0;
int p1scoretemp = 0, p2scoretemp = 0;

int maxScore = 10;
sf::Clock inputClock;

int menuSelection = 0, total = 3;

sf::Texture texture, texture1, gbgTexture;
sf::Sprite sprite, sprite1, gbgSprite;

sf::SoundBuffer scrollBuffer, selectBuffer, escapekBuffer, hitPaddleBuffer, winBuffer, bgmBuffer;
sf::Sound scrollSound, selectSound, escapeSound, hitPaddleSound, winSound;
sf::Music bgm;

sf::Texture intro1T, intro2T, intro3T, intro4T, intro5T, intro6T, intro7T, intro8T, intro9T, intro10T, intro11T;
sf::Sprite intro1S;
sf::SoundBuffer sbIntro;
sf::Sound sIntro;

std::string username = "";
sf::Text userInputText;

std::vector<std::string> names;
std::vector<int> scores;
int lead = 0;

int mode = 0;

View view;
Clock shakeClock;
bool isShaking = false;
float shakeDuration = 0.2f; // 0.5 seconds
float shakePixels = 4.0f;
Vector2f viewCenter;