#pragma once
#include "initialize.h"
#include <vector>
#include <string>

extern int width, height;
extern int gameState, check;
extern Font font, heading;
extern float hR, hG, hB, hSize;

extern bool player1, player2;
extern int p1Score, p2Score;

extern float p1tempX, p1tempY, p2tempX, p2tempY, balltempX, balltempY, ballspeedX, ballspeedY;
extern int p1scoretemp, p2scoretemp;
extern int maxScore;
extern Clock inputClock;
extern int menuSelection, total;

extern Texture texture, texture1, gbgTexture;
extern Sprite sprite, sprite1, gbgSprite;

extern SoundBuffer scrollBuffer, selectBuffer, escapekBuffer, hitPaddleBuffer, winBuffer, bgmBuffer;
extern Sound scrollSound, selectSound, escapeSound, hitPaddleSound, winSound;
extern Music bgm;

extern Texture intro1T, intro2T, intro3T, intro4T, intro5T, intro6T, intro7T, intro8T, intro9T, intro10T, intro11T;
extern Sprite intro1S;
extern SoundBuffer sbIntro;
extern Sound sIntro;

extern string username;
extern Text userInputText;

extern vector<std::string> names;
extern vector<int> scores;
extern int lead;
extern int mode;

extern View view;
extern Clock shakeClock;
extern bool isShaking;
extern float shakeDuration;
extern float shakePixels;
extern Vector2f viewCenter;

extern int pR, pG, pB;