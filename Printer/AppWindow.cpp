#include "Application.h"

#include <stdio.h>
#include <File.h>
#include <Font.h>
#include <Rect.h>
#include <String.h>


AppView::AppView (BRect frame)
	: BTextView (frame, "main View", frame.InsetByCopy(5, 5),
	  B_FOLLOW_ALL_SIDES,
	  B_WILL_DRAW | B_NAVIGABLE)
{
	this->SetViewColor (ui_color(B_MENU_BACKGROUND_COLOR));
	this->MakeEditable();
	this->MakeResizable (false);
	this->SetFont (be_fixed_font, B_FONT_FAMILY_AND_STYLE);
//	this->SetHighColor (0, 0, 0);
}


AppView::~AppView()
{
	this->Erase();
}


void AppView::Erase (void)
{
	SetText("");
}


void AppView::AddText (const char* text, int32 numBytes = 0)
{
	if (text)
	{
		BString str(this->Text());
		if (numBytes == 0)
			str.Append (text);
		else
			str.Append (text, numBytes);
		str.Append ("\n");
		SetText (str.String());
	}
}



void AppView::SaveMessage(BMessage* msg, const char* fileName)
{
	if (!msg || !fileName) return;
	BFile* toSave = new BFile(fileName, B_WRITE_ONLY | B_CREATE_FILE | B_ERASE_FILE);
	if (toSave->InitCheck() == B_OK)
	{
		msg->Flatten(toSave, 0);
		toSave->Unset();
		AddText ("Message saved!");
	}
	delete toSave;
}


void AppView::KeyDown(const char *bytes, int32 numBytes)
{
	BMessage *msg = Window()->CurrentMessage();
	BString filePath;
	BFile* toSave;
	if (numBytes == 1)
	{
		switch (bytes[0])
		{
			case B_FUNCTION_KEY:
				if ( msg ) {
					int32 key;
					char c[100];
					msg->FindInt32("key", &key);
					switch ( key ) {
						case B_F1_KEY:
							AddText ("F1");
							break;
						case B_F2_KEY:
							AddText ("F2");
							break;
						case B_F3_KEY:
							AddText ("F3");
							break;
						case B_F4_KEY:
							AddText ("F4");
							break;
						case B_F5_KEY:
							AddText ("F5");
							break;
						case B_F6_KEY:
							AddText ("F6");
							break;
						case B_F7_KEY:
							AddText ("F7");
							break;
						case B_F8_KEY:
							AddText ("F8");
							break;
						case B_F9_KEY:
							AddText ("F9");
							break;
						case B_F10_KEY:
							AddText ("F10");
							break;
						case B_F11_KEY:
							AddText ("F11");
							break;
						case B_F12_KEY:
							AddText ("F12");
							break;
						case B_PRINT_KEY:
							AddText ("Print Screen");
							break;
						case B_SCROLL_KEY:
							AddText ("Scroll Lock");
							break;
						case B_PAUSE_KEY:
							AddText ("Pause / Break");
							break;
						default:
							sprintf (c, "0x%04X", key);
							AddText (c);
							break;
					};	// <-- end of switch
				}	// <-- end of if
				break;
			case B_BACKSPACE:
				AddText ("Backspace");
				
				SaveMessage(msg, "Backspace.msg");
				
				break;
			case B_ENTER:
				AddText ("Enter key");
				break;
			case B_SPACE:
				AddText ("Spacebar");
				break;
			case B_TAB:
				AddText ("Tab");
				break;
			case B_ESCAPE:
				AddText ("Escape");
				break;
			case B_LEFT_ARROW:
				AddText ("Arrow left");
				break;
			case B_RIGHT_ARROW:
				AddText ("Arrow right");
				break;
			case B_UP_ARROW:
				AddText ("Arrow up");
				break;
			case B_DOWN_ARROW:
				AddText ("Arrow down");
				break;
			case B_INSERT:
				AddText ("Insert");
				break;
			case B_DELETE:
				AddText ("Delete");
				break;
			case B_HOME:
				AddText ("Home");
				break;
			case B_END:
				AddText ("End");
				break;
			case B_PAGE_UP:
				AddText ("Page up");
				break;
			case B_PAGE_DOWN:
				AddText ("Page down");
				break;
			default:
				AddText (bytes, numBytes);
		};
	}
	else
	{
		if (numBytes > 0) {
			AddText (bytes, numBytes);
			
			BString receivedValue(bytes);
			if 		(receivedValue.Compare("Ĉ") == 0) {
				SaveMessage(msg, "Ĉ.msg");
			}
			else if (receivedValue.Compare("ĉ") == 0) {
				SaveMessage(msg, "ĉ.msg");
			}
			else if (receivedValue.Compare("Ŝ") == 0) {
				SaveMessage(msg, "Ŝ.msg");
			}
			else if (receivedValue.Compare("ŝ") == 0) {
				SaveMessage(msg, "ŝ.msg");
			}
			else if (receivedValue.Compare("Ĵ") == 0) {
				SaveMessage(msg, "Ĵ.msg");
			}
			else if (receivedValue.Compare("ĵ") == 0) {
				SaveMessage(msg, "ĵ.msg");
			}
			else if (receivedValue.Compare("Ĥ") == 0) {
				SaveMessage(msg, "Ĥ.msg");
			}
			else if (receivedValue.Compare("ĥ") == 0) {
				SaveMessage(msg, "ĥ.msg");
			}
			else if (receivedValue.Compare("Ĝ") == 0) {
				SaveMessage(msg, "Ĝ.msg");
			}
			else if (receivedValue.Compare("ĝ") == 0) {
				SaveMessage(msg, "ĝ.msg");
			}
			else if (receivedValue.Compare("Ŭ") == 0) {
				SaveMessage(msg, "Ŭ.msg");
			}
			else if (receivedValue.Compare("ŭ") == 0) {
				SaveMessage(msg, "ŭ.msg");
			}
			
		}
	}
}


void AppView::AttachedToWindow()
{
	this->SetViewColor (ui_color(B_MENU_BACKGROUND_COLOR));
	this->MakeEditable();
	this->MakeResizable (false);
	this->SetFont (be_fixed_font);
	this->SetHighColor (ui_color (B_MENU_SELECTED_ITEM_TEXT_COLOR));
	BView::AttachedToWindow();
}

/************************/

AppWindow::AppWindow () :
	BWindow (BRect (100, 100, 500, 500),
			 "ScanCode",
			 B_DOCUMENT_WINDOW,
			 B_NOT_ZOOMABLE | B_NOT_RESIZABLE | B_ASYNCHRONOUS_CONTROLS)
{
	mainView = new AppView (Bounds());
	AddChild (mainView);
}


AppWindow::~AppWindow ()
{
	if (mainView) {
		this->LockLooper();
		mainView->RemoveSelf();
		delete (mainView);
		mainView = NULL;
	}
}


void AppWindow::WindowActivated (bool active)
{
	BWindow::WindowActivated(active);
	if (active && mainView)
		mainView->Erase();
}


void AppWindow::MessageReceived (BMessage *in)
{
	BWindow::MessageReceived(in);
	switch (in->what)
	{
		case B_UNMAPPED_KEY_DOWN:
			int32 key;
			char c[10];
			in->FindInt32 ("key", &key);
			sprintf (c, "0x%08X", (uint32)key);
			mainView->AddText (c);
			break;
		case B_MODIFIERS_CHANGED:
/*
			int32 currentModifiers, oldModifiers, value;
			in->FindInt32 ("modifiers", &currentModifiers);
			in->FindInt32 ("be:old_modifiers", &oldModifiers);
			value = currentModifiers ^ oldModifiers; // Only changed values
			value ^= oldModifiers;	// Only new values;
			if (value & B_SHIFT_KEY)
				mainView->AddText ("Shift");
			if (value & B_COMMAND_KEY)
				mainView->AddText ("Command");
			if (value & B_CONTROL_KEY)
				mainView->AddText ("Control");
			if (value & B_OPTION_KEY)
				mainView->AddText ("Option");
			if (value & B_MENU_KEY)
				mainView->AddText ("Menu");
			if (value & B_CAPS_LOCK)
				mainView->AddText ("Caps Lock");
			if (value & B_NUM_LOCK)
				mainView->AddText ("Num Lock");
			if (value & B_SCROLL_LOCK)
				mainView->AddText ("Scroll Lock");
*/
			break;
	};

}
