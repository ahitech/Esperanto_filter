#ifndef APPLICATION_H
#define APPLICATION_H

#include <Application.h>
#include <Window.h>
	
#include "AppWindow.h"

class MainApp : public BApplication
{
	public:
		MainApp ();
		~MainApp();

		void ReadyToRun();

	private:
		AppWindow *window;

};


int main (void);

#endif	// <-- APPLICATION_H
