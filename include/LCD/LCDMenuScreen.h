/*
 * LCDMenuScreen.h
 *
 *  Created on: Apr 6, 2015
 *      Author: guywenck
 */

#ifndef LCDMENUSCREEN_H_
#define LCDMENUSCREEN_H_

#include "../TRL/TRL_BaseIncludes.h"
#include "LCD.h"

#define NO_SCREEN 0

namespace TRL {
	class LCDMenuScreen {
	protected:
		LCDMenuScreen* homeScreen;
		LCDMenuScreen* leftScreen;
		LCDMenuScreen* enterScreen;
		LCDMenuScreen* rightScreen;

		LCD* lcd;
	public:
		LCDMenuScreen();
		LCDMenuScreen(LCD* lcd);
		virtual ~LCDMenuScreen();

		void setReferencedScreens(LCDMenuScreen* homeScreen, LCDMenuScreen* leftScreen, LCDMenuScreen* enterScreen, LCDMenuScreen* rightScreen);
		void setDisplayLCD(LCD* lcd);

		static void setScreenArrayLCD(int size, LCDMenuScreen** screens, LCD* lcd);
		static void setScreenArrayLCD(Array<LCDMenuScreen*>* screens, LCD* lcd);

		bool hasLeftScreen();
		bool hasEnterScreen();
		bool hasRightScreen();

		LCDMenuScreen* getLeftScreen();
		LCDMenuScreen* getEnterScreen();
		LCDMenuScreen* getRightScreen();


		virtual void display() = 0;

		//return this if you wish to stay on the current menu screen
		//override these functions in the child class to alter their functionality
		//they will automatically get called by the LCDMenuHandler Class
		virtual LCDMenuScreen* onShortLeftButtonPress();
		virtual LCDMenuScreen* onShortCenterButtonPress();
		virtual LCDMenuScreen* onShortRightButtonPress();

		virtual LCDMenuScreen* onLongLeftButtonPress();
		virtual LCDMenuScreen* onLongCenterButtonPress();
		virtual LCDMenuScreen* onLongRightButtonPress();

		void printRefrences();
	};
}

#endif /* LCDMENUSCREEN_H_ */
