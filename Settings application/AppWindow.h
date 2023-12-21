/*
 * Copyright 2024, Alex Hitech <ahitech@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef APP_WINDOW_H
#define APP_WINODW_H

#include <View.h>
#include <Rect.h>
#include <Window.h>
#include <SupportDefs.h>

#include "MainView.h"

class AppWindow : public BWindow
{
	public:
		AppWindow();
		~AppWindow();
		
		void WindowActivated(bool);
		void MessageReceived (BMessage* in);
		bool QuitRequested();
	private:
		MainView* mainView;
};



#endif // APP_WINODW_H
