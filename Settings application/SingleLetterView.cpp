/*
 * Copyright 2024, Alexey Burshtein <ahitech@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#include "SingleLetterView.h"

#include <cctype>


#include <InterfaceDefs.h>
#include <Region.h>
#include <Window.h>

#define		INSET		3


SingleLetterView::SingleLetterView(const char* name, char letter)
	: BTextView(name),
	  bViewBitmapIsSet(false),
	  internalBitmap(NULL),
	  internalView(NULL)
{
	MakeResizable(false);
	SetAlignment(B_ALIGN_CENTER);
	SetStylable(false);	// We don't need styles here
	SetMaxBytes(1);		// Max of 1 unicode character
	if (letter != 0) { SetText(&letter, 1); }
	SetWordWrap(false);
	SetTabWidth(0);
	ResizeToPreferred();
	
	internalView = new BView(Bounds(), "Cross", B_FOLLOW_ALL_SIDES, B_WILL_DRAW | B_FULL_UPDATE_ON_RESIZE | B_FRAME_EVENTS);
	internalBitmap = new BBitmap(Bounds(), B_RGBA32, true, true);
	if (internalBitmap && internalView) {
		internalBitmap->AddChild(internalView);
		if (internalBitmap->Lock())
		{
			BRegion internalRegion(this->Bounds());
			internalView->SetLowColor(0, 0, 0);
			internalView->SetHighColor(255, 0, 0);
			internalView->FillRegion(new BRegion(internalBitmap->Bounds()), B_SOLID_LOW);
			BPoint upperRight(internalBitmap->Bounds().Width() - 2, 2);
			BPoint bottomLeft(2, internalBitmap->Bounds().Height() - 2);
			SetPenSize(3.0);		// The line is thick
			internalView->StrokeLine(upperRight, bottomLeft);
			internalView->Sync();
			internalBitmap->Unlock();
		}
		internalBitmap->RemoveChild(internalView);
	}

//	if (letter) {
//		SetActive(false);
//	} else {
//		SetActive(true);
//	}
}

SingleLetterView::~SingleLetterView() {
//	if (internalBitmap) { 
//		if (bViewBitmapIsSet) ClearViewBitmap();
//		delete (internalBitmap);
//		internalBitmap = NULL;
//	}
//	if (internalView) {
//		delete (internalView);
//		internalView = NULL;
//	}
	
}


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
		*width = rectangle[0].Width();
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
	BTextView::MakeFocus(flag);
	if (!flag) {
		char currentChar = GetCharacter();
		if (!std::isalnum(currentChar)) {
			SetActive(false);
		}	
	} else {			// The view became active
		SetActive(true);
	}
	BTextView::Invalidate();
}

void SingleLetterView::ResizeToPreferred() {
	float width = 0, height = 0;
	this->GetPreferredSize(&width, &height);
	this->ResizeTo(width + 2*INSET, this->LineHeight() + 2*INSET);
	this->SetTextRect(BRect(1, 1, width-1, height-1));
}


void SingleLetterView::KeyDown(const char *bytes, int32 numBytes)
{
		SelectAll();
		Delete();		// Clear old text

	// The only allowed characters are the non-whitespace ones
	if (numBytes == 1) {
//		this->SetViewColor(linkHover);
		if (bytes) {
			if ((bytes[0] == B_BACKSPACE) ||
				(bytes[0] == B_DELETE))
			{
				BTextView::KeyDown(bytes, numBytes);
			}
			if (std::isalnum(bytes[0])) {
				SetActive(true);
				BTextView::KeyDown(bytes, numBytes);
			} else {
//				SetActive(false);
			}
		}
	}
}

void SingleLetterView::SetActive(bool flag)
{
	// Basically, what I want to do is to draw a thick red line from the
	// upper right corner of the control's frame to the bottom left corner
	
#if 0	
	static rgb_color currentHighColor = HighColor();
	static rgb_color transparent = ui_color(B_CONTROL_BACKGROUND_COLOR);
			
	SetLineMode(B_ROUND_CAP, B_ROUND_JOIN);
	BRect bounds = Bounds();
	BPoint upperRight(bounds.Width() - 2, 2);
	BPoint bottomLeft(2, bounds.Height() - 2);
	if (!flag) {			// Crossing out the window if it should NOT be active
		SetPenSize(3.0);		// The line is thick
		SetHighColor(255, 0, 0);	// The line is red
		SelectAll();
		Delete();
	} else {
		SetPenSize(5.0);		// The line is thick
		SetHighColor(ui_color(B_DOCUMENT_BACKGROUND_COLOR));
	}
	StrokeLine(upperRight, bottomLeft);
	SetHighColor(currentHighColor);
#endif

	if (flag) {
		if (this->bViewBitmapIsSet) {
			this->ClearViewBitmap();
			this->bViewBitmapIsSet = false;
		}	
	} else {
		DrawBitmap(internalBitmap);
		this->bViewBitmapIsSet = true;
	}
	
}

char SingleLetterView::GetCharacter()
{
	return (char )this->ByteAt(0);
}
