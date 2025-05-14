#pragma once
#include "./initialize.h"
void setTextinMiddle(Text& text) {
	float x = text.getGlobalBounds().width / 2, y = text.getGlobalBounds().height / 2;
	text.setOrigin(v(x, y));
	text.setPosition(v((float)(width / 2), (float)(height / 2)));
}
// Check if a JoyStick Button is Pressed
bool isJoystickButtonPressed(unsigned int joystickId, unsigned int button) {
	return Joystick::isConnected(joystickId) && Joystick::isButtonPressed(joystickId, button);
}

// Check if a JoyStick Axis is Movedd
bool checkAxisMoved(unsigned int id, bool Up, bool Down) {
	if (!Joystick::isConnected(id)) {
		return false;
	}
	float threshold = 15.0f;
	float stickX = Joystick::getAxisPosition(id, Joystick::X);
	float stickY = Joystick::getAxisPosition(id, Joystick::Y);
	if (abs(stickY) < threshold || abs(stickX) >= abs(stickY)) {
		return false;
	}
	if (Up && stickY < -99.0f)
		return true;
	if (Down && stickY > 99.0f)
		return true;

	return false;
}
void startShakeEffect() {
	isShaking = true;
	shakeClock.restart();
}

void endShakeEffect() {
	if (isShaking) {
		if (shakeClock.getElapsedTime().asSeconds() < shakeDuration) {
			float offsetX = (rand() % 100 / 100.f - 0.5f) * 2 * shakePixels;
			float offsetY = (rand() % 100 / 100.f - 0.5f) * 2 * shakePixels;
			view.setCenter(viewCenter + Vector2f(offsetX, offsetY));
		}
		else {
			isShaking = false;
			view.setCenter(viewCenter);
			view.setRotation(0);
		}
	}

}