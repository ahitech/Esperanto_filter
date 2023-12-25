/*
 * Copyright 2024, Alexey Burshtein <ahitech@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#include "SingleLetterView.h"

#include <cctype>


#define		INSET		3


SingleLetterView::SingleLetterView(const char* name, char letter = 0)
	: BTextView(name)
{
	MakeResizable(false);
	SetAlignment(B_ALIGN_CENTER);
	SetStylable(false);	// We don't need styles here
	SetMaxBytes(2);		// Max of 1 unicode character
	if (letter != 0) { SetText(&letter, 1); }
	SetWordWrap(false);
	ResizeToPreferred();
}

SingleLetterView::~SingleLetterView() { }	// Nothing to do here


void SingleLetterView::AttachedToWindow()
{
   if ( Parent() )
   {
//      SetViewColor(Parent()->ViewColor());
   }
   BView::AttachedToWindow();
}


void SingleLetterView::GetPreferredSize(float *width, float *height)
{
	BFont usedFont;
	GetFontAndColor(0, &usedFont);
	BRect rectangle[2];
	if (width && height)
	{
		usedFont.GetBoundingBoxesAsGlyphs("W",		// Widest character in western fonts
					1, B_SCREEN_METRIC, rectangle);
		*width = rectangle[0].Width() + 2*INSET;
//		*height = rectangle[0].Height() + 2*INSET;
		*height = this->LineHeight();
	}
}


void SingleLetterView::GetPreferredSize(BRect* rect)
{
	if (rect)
	{
		float width = 0, height = 0;
		this->GetPreferredSize(&width, &height);
		rect->Set(0, 0, width, height);
	}	
	
}


void SingleLetterView::ResizeToPreferred() {
	float width = 0, height = 0;
	this->GetPreferredSize(&width, &height);
	this->ResizeTo(width, this->LineHeight());
}


void SingleLetterView::KeyDown(const char *bytes, int32 numBytes)
{
	// The only allowed characters are the non-whitespace ones
	if (numBytes == 1 && bytes && std::isalnum(bytes[0])) {
		SelectAll();
		Delete();		// Clear old text
		
		BTextView::KeyDown(bytes, numBytes);
	}
}

char SingleLetterView::GetCharacter()
{
	return (char )this->ByteAt(0);
}
