/*
 * Copyright 2024, Alexey Burshtein <ahitech@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef SINGLE_LETTER_VIEW_H
#define SINGLE_LETTER_VIEW_H

#include <TextView.h>

#include <SupportDefs.h>


class SingleLetterView
	: public BTextView
{
public:
	SingleLetterView(const char* name, char letter = 0);
	virtual ~SingleLetterView();
	
	virtual char GetCharacter();
	virtual void GetPreferredSize(float* width, float* height);
	virtual void GetPreferredSize(BRect *rect);
	virtual void ResizeToPreferred();
	virtual void MakeFocus(bool flag = true);
	virtual void MouseDown(BPoint point);
	
	virtual void AttachedToWindow();
	// virtual void MessageReceived(BMessage* message);
	virtual void KeyDown(const char* bytes, int32 numBytes);

protected:
	virtual void SetActive(bool flag = false);
};


#endif // SINGLE_LETTER_VIEW_H
