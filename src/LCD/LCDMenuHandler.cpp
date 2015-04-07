/*
 * LCDMenuHandler.cpp
 *
 *  Created on: Apr 6, 2015
 *      Author: guywenck
 */

#include "LCD/LCDMenuHandler.h"

namespace TRL {

void LCDMenuHandler::initScreens() {
	//Initialize Screens
	mainAutonScreen = new LCDMainAutonScreen();
	mainBatteryScreen = new LCDMainBatteryScreen();
	mainSensorScreen = new LCDMainSensorScreen();
}

LCDMenuHandler::~LCDMenuHandler() {
	//Delete any screens you initialized
	delete mainAutonScreen;
	delete mainBatteryScreen;
	delete mainSensorScreen;
}

void LCDMenuHandler::initScreenRelationships(LCD *displayLCD) {
	lcd = displayLCD;

	//Set the relationships between menus
	mainAutonScreen->setReferencedScreens(mainAutonScreen, mainBatteryScreen,
			mainAutonScreen, mainSensorScreen);

	mainBatteryScreen->setReferencedScreens(mainAutonScreen, mainSensorScreen,
			mainBatteryScreen, mainAutonScreen);

	mainSensorScreen->setReferencedScreens(mainAutonScreen, mainAutonScreen,
			mainSensorScreen, mainBatteryScreen);

	//Set the start screen
	currentScreen = mainAutonScreen;
}

//the parameter is a requirement of a function being run in its own task
void LCDMenuHandler::run(void* lcdMenuHandlerInstance) {
	LCDMenuHandler* handler = (LCDMenuHandler*) lcdMenuHandlerInstance;
	handler->lcd->enableBacklight();
	while (handler->running) {
		println("TIME: %d", millis());
		handler->updateLCDButtonPresses();
		if (handler->currentScreen == NULL) {
			println(ERROR, "LCDMenuHandler", "run",
					"The pointer currentScreen has a null value. Aborting the task.");
			handler->stop();
			break;
		} else {
			handler->currentScreen->display(handler->lcd);	//Memory leak is in here
		}

		if (handler->wasShortLeftJustReleased) {
			handler->currentScreen =
					handler->currentScreen->onShortLeftButtonPress();
			handler->updateLCDButtonPresses();
		} else if (handler->wasShortCenterJustReleased) {
			handler->currentScreen =
					handler->currentScreen->onShortCenterButtonPress();
			handler->updateLCDButtonPresses();
		} else if (handler->wasShortRightJustReleased) {
			handler->currentScreen =
					handler->currentScreen->onShortRightButtonPress();
			handler->updateLCDButtonPresses();
		} else if (handler->wasLeftJustNowLongPressed) {
			handler->currentScreen =
					handler->currentScreen->onLongLeftButtonPress();
			handler->updateLCDButtonPresses();
		} else if (handler->wasCenterJustNowLongPressed) {
			handler->currentScreen =
					handler->currentScreen->onLongCenterButtonPress();
			handler->updateLCDButtonPresses();
		} else if (handler->wasRightJustNowLongPressed) {
			handler->currentScreen =
					handler->currentScreen->onLongRightButtonPress();
			handler->updateLCDButtonPresses();
		}

//		delay(20);
	}
}

void LCDMenuHandler::start(LCD* lcd) {
	initScreenRelationships(lcd);
	if (running) {
		return;
	}
	running = true;
	println("This1: %d", this);
	taskHandle = taskCreate(run, TASK_DEFAULT_STACK_SIZE, this,
			TASK_PRIORITY_DEFAULT + 1);
}

void LCDMenuHandler::stop() {
	if (!running) {
		return;
	}
	running = false;
	taskDelete(taskHandle);
}

void LCDMenuHandler::updateLCDButtonPresses() {
	wasLeftPressed = isLeftPressed;
	wasCenterPressed = isCenterPressed;
	wasRightPressed = isRightPressed;

	wasLeftLongPressed = isLeftLongPressed;
	wasCenterLongPressed = isCenterLongPressed;
	wasRightLongPressed = isRightLongPressed;

	isLeftPressed = lcd->isOnlyLeftButtonPressed();
	isCenterPressed = lcd->isOnlyCenterButtonPressed();
	isRightPressed = lcd->isOnlyRightButtonPressed();

	wasShortLeftJustReleased = false;
	wasShortCenterJustReleased = false;
	wasShortRightJustReleased = false;

	wasLongLeftJustReleased = false;
	wasLongCenterJustReleased = false;
	wasLongRightJustReleased = false;

	wasLeftJustNowLongPressed = false;
	wasCenterJustNowLongPressed = false;
	wasRightJustNowLongPressed = false;

	//Start a timer if a button has just been pressed
	if (!wasLeftPressed && isLeftPressed) {
		leftPressedAt = millis();
	} else if (wasLeftPressed && !isLeftPressed) {
		leftPressedAt = 0;
		if (isLeftLongPressed) {
			wasLongLeftJustReleased = true;
		} else {
			wasShortLeftJustReleased = true;
		}
	}

	if (!wasCenterPressed && isCenterPressed) {
		centerPressedAt = millis();
	} else if (wasCenterPressed && !isCenterPressed) {
		centerPressedAt = 0;
		if (isCenterLongPressed) {
			wasLongCenterJustReleased = true;
		} else {
			wasShortCenterJustReleased = true;
		}
	}

	if (!wasRightPressed && isRightPressed) {
		rightPressedAt = millis();
	} else if (wasRightPressed && !isRightPressed) {
		rightPressedAt = 0;
		if (isRightLongPressed) {
			wasLongRightJustReleased = true;
		} else {
			wasShortRightJustReleased = true;
		}
	}

	//Dettermine if a long press has been initiated
	if (leftPressedAt != 0) {
		if ((millis() - leftPressedAt) >= LCD_LONG_PRESS_TIME_MILLIS) {
			isLeftLongPressed = true;
		}
	} else {
		isLeftLongPressed = false;
	}

	if (centerPressedAt != 0) {
		if ((millis() - centerPressedAt) >= LCD_LONG_PRESS_TIME_MILLIS) {
			isCenterLongPressed = true;
		}
	} else {
		isCenterLongPressed = false;
	}

	if (rightPressedAt != 0) {
		if ((millis() - rightPressedAt) >= LCD_LONG_PRESS_TIME_MILLIS) {
			isRightLongPressed = true;
		}
	} else {
		isRightLongPressed = false;
	}

	//Long Presses Handling
	if (!wasLeftLongPressed && isLeftLongPressed) {
		wasLeftJustNowLongPressed = true;
	}

	if (!wasCenterLongPressed && isCenterLongPressed) {
		wasCenterJustNowLongPressed = true;
	}

	if (!wasRightLongPressed && isRightLongPressed) {
		wasRightJustNowLongPressed = true;
	}
}

LCDMenuHandler::LCDMenuHandler() {
	taskHandle = NULL;
	running = NULL;
	lcd = NULL;

	wasLeftPressed = false;
	isLeftPressed = false;
	wasLeftLongPressed = false;
	isLeftLongPressed = false;
	wasLeftJustNowLongPressed = false;
	wasShortLeftJustReleased = false;
	wasLongLeftJustReleased = false;
	leftPressedAt = 0;

	wasCenterPressed = false;
	isCenterPressed = false;
	wasCenterLongPressed = false;
	isCenterLongPressed = false;
	wasCenterJustNowLongPressed = false;
	wasShortCenterJustReleased = false;
	wasLongCenterJustReleased = false;
	centerPressedAt = 0;

	wasRightPressed = false;
	isRightPressed = false;
	wasRightLongPressed = false;
	isRightLongPressed = false;
	wasRightJustNowLongPressed = false;
	wasShortRightJustReleased = false;
	wasLongRightJustReleased = false;
	rightPressedAt = 0;

	initScreens();
}

} /* namespace TRL */
