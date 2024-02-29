#include <stdio.h>
#include <Font.h>
#include <Rect.h>
#include <String.h>


#include "MainView.h"
#include "Application.h"

AppWindow::AppWindow () :
	BWindow (BRect (100, 100, 500, 500),
			 "Esperanto Filter",
			 B_TITLED_WINDOW,
			 B_ASYNCHRONOUS_CONTROLS)
{
	mainView = new MainView (Bounds());
	AddChild (mainView);
}


bool AppWindow::QuitRequested() {
	be_app->PostMessage(B_QUIT_REQUESTED);
	return BWindow::QuitRequested();
}


AppWindow::~AppWindow ()
{

}


void AppWindow::WindowActivated (bool active)
{
	BWindow::WindowActivated(active);
}


void AppWindow::MessageReceived (BMessage *in)
{
	BWindow::MessageReceived(in);
	switch (in->what)
	{
		case B_UNMAPPED_KEY_DOWN:
			break;
		case B_MODIFIERS_CHANGED:
			break;
	};
}
