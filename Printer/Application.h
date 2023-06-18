#ifndef APPLICATION_H
#define APPLICATION_H

#include <Application.h>
#include <TextView.h>
#include <Rect.h>
#include <Window.h>

class AppView : public BTextView
{
	public:
		AppView(BRect );
		~AppView ();
		
		void Erase ();
		void AddText (const char *text, int32 numBytes);
		void KeyDown(const char *bytes, int32 numBytes);
		void AttachedToWindow();
		
};

class AppWindow : public BWindow
{
	public:
		AppWindow();
		 ~AppWindow();
		
		void WindowActivated(bool);
		void MessageReceived (BMessage* in);
	private:
		AppView *mainView;
};
	

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
