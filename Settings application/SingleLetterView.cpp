/*
 * Copyright 2024, Alexey Burshtein <ahitech@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#include "SingleLetterView.h"

#include <cctype>

#include <InterfaceDefs.h>
#include <Window.h>

#define		INSET		3


SingleLetterView::SingleLetterView(const char* name, char letter = 0)
	: BTextView(name)
{
	MakeResizable(false);
	SetAlignment(B_ALIGN_CENTER);
	SetStylable(false);	// We don't need styles here
	SetMaxBytes(1);		// Max of 1 unicode character
	if (letter != 0) { SetText(&letter, 1); }
	SetWordWrap(false);
	SetTabWidth(0);
	ResizeToPreferred();
	
	if (letter) {
		SetActive(false);
	} else {
		SetActive(true);
	}
}

SingleLetterView::~SingleLetterView() { }	// Nothing to do here


void SingleLetterView::AttachedToWindow()
{
   	if ( Parent() )
	{
		Invalidate();
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

void SingleLetterView::MakeFocus(bool flag)
{
	if (!flag) {
		char currentChar = GetCharacter();
		if (!std::isalnum(currentChar)) {
			SetActive(false);
		}	
	} else {			// The view became active
		SetActive(true);
	}	
	BTextView::MakeFocus(flag);
}

void SingleLetterView::MouseDown(BPoint point)
{
//	SetViewColor(controlBackground);
//	SetActive(true);
	BTextView::MouseDown(point);
}

void SingleLetterView::ResizeToPreferred() {
	float width = 0, height = 0;
	this->GetPreferredSize(&width, &height);
	this->ResizeTo(width, this->LineHeight());
	this->SetTextRect(BRect(0, 0, width-1, height-1));
}


void SingleLetterView::KeyDown(const char *bytes, int32 numBytes)
{
	rgb_color linkHover = ui_color(B_LINK_HOVER_COLOR),
			  controlBackground = ui_color(B_CONTROL_BACKGROUND_COLOR);
	
	// The only allowed characters are the non-whitespace ones
	if (numBytes == 1) {
		SelectAll();
		Delete();		// Clear old text
//		this->SetViewColor(linkHover);
		if (bytes) {
			if (std::isalnum(bytes[0])) {
				SetActive(true);
			} else {
				SetActive(false);
			}
		}
		BTextView::KeyDown(bytes, numBytes);
//			this->SetViewColor(controlBackground);
	}
}

void SingleLetterView::SetActive(bool flag)
{
	// Basically, what I want to do is to draw a thick red line from the
	// upper right corner of the control's frame to the bottom left corner
	static rgb_color currentHighColor = HighColor();
	static rgb_color transparent = ui_color(B_CONTROL_BACKGROUND_COLOR);
//	if (Window()->LockLooper())
	{
		SetPenSize(3.0);		// The line is thick
		SetLineMode(B_ROUND_CAP, B_ROUND_JOIN);
		BRect bounds = Bounds();
		BPoint upperRight(bounds.Width() - 2, 2);
		BPoint bottomLeft(2, bounds.Height() - 2);
		if (!flag) {			// Crossing out the window if it should NOT be active
			SetHighColor(255, 0, 0);	// The line is red
		} else {
			SetHighColor(transparent);
		}
		StrokeLine(upperRight, bottomLeft);
//		Window()->UpdateIfNeeded();
		SetHighColor(currentHighColor);
//		Window()->UnlockLooper();
	}
}

char SingleLetterView::GetCharacter()
{
	return (char )this->ByteAt(0);
}
