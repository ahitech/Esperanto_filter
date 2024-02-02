/*
 * Copyright 2024, Alexey Burshtein <ahitech@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#include "SingleLetterView.h"

#include <cctype>


#include <InterfaceDefs.h>
#include <Region.h>
#include <Window.h>

#include "MainView.h"

#define		INSET		3

//	#pragma mark - Constructor and destructor

SingleLetterView::SingleLetterView(const char* name, char letter)
	: BTextView(name),
	  enabled(true),
	  internalView(NULL),
	  internalBitmap(NULL)
{
	MakeResizable(false);
	SetAlignment(B_ALIGN_CENTER);
	SetStylable(false);	// We don't need styles here
	SetMaxBytes(1);		// Max of 1 unicode character
	if (letter != 0) { SetText(&letter, 1); }
	SetWordWrap(false);
	SetTabWidth(0);
	ResizeToPreferred();
	CreateBitmap();
}

SingleLetterView::~SingleLetterView() {
	if (internalView) {
		internalView->RemoveSelf();
		delete internalView;
		internalView = NULL;
	}
	if (internalBitmap)
	{
//		ClearViewBitmap();
		delete internalBitmap;
		internalBitmap = NULL;
	}
}

//	#pragma mark - Member functions
void SingleLetterView::CreateBitmap(void)
{
	BRect bounds = Bounds(),
		  smallRect = bounds.InsetBySelf(INSET, INSET);
	internalBitmap = new BBitmap(smallRect,
								 B_RGB32, 
								 true,		// Accept views
								 false);	// Contiguous memory is not needed
	internalView = new BView(smallRect, "Disabler", B_FOLLOW_NONE, B_WILL_DRAW);
	
	if (internalBitmap && internalView)
	{
		internalBitmap->AddChild(internalView);
		internalBitmap->Lock();
		internalView->SetLowColor(ui_color(B_CONTROL_BACKGROUND_COLOR));
		internalView->SetHighColor(255, 0, 0);
		BRegion region(internalView->Bounds());
		internalView->FillRegion(&region, B_SOLID_LOW);
		BPoint upperRight(smallRect.Width(), 0);
		BPoint bottomLeft(0, smallRect.Height());
		internalView->SetPenSize(INSET);			// The line is thick
		internalView->StrokeLine(upperRight, bottomLeft);
		internalView->Sync();
		
		internalBitmap->RemoveChild(internalView);
		internalBitmap->Unlock();
	}
}


void SingleLetterView::AttachedToWindow()
{
   	if ( Parent() )
	{
		Invalidate();
	}
	BView::AttachedToWindow();
	
	SetActive(GetCharacter() != 0);
	Invalidate();
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
		*width = rectangle[0].Width()* 2;
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
	if (!enabled) return;	// Don't accept focus if the view is disabled
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


void SingleLetterView::ResizeToPreferred() {
	float width = 0, height = 0;
	this->GetPreferredSize(&width, &height);
	this->ResizeTo(width + 2*INSET, this->LineHeight() + 2*INSET);
	this->SetTextRect(BRect(1, 1, width-1, height-1));
}


void SingleLetterView::KeyDown(const char *bytes, int32 numBytes)
{
	rgb_color linkHover = ui_color(B_LINK_HOVER_COLOR),
			  controlBackground = ui_color(B_CONTROL_BACKGROUND_COLOR);
	
	// The only allowed characters are the non-whitespace ones
	if (numBytes == 1) {
		SelectAll();
		Delete();		// Clear old text
		Invalidate();
//		this->SetViewColor(linkHover);
		if (bytes) {
			if ((bytes[0] == B_BACKSPACE) ||
				(bytes[0] == B_DELETE))
			{
				BTextView::KeyDown(bytes, numBytes);
			}
			if (std::isalnum(bytes[0])) {
				char lowercase[2];
				lowercase[0] = tolower(bytes[0]);
				lowercase[1] = '\0';
				if (Parent())
				{
					SingleLetterView* SLVpointer = NULL;
					BView* sibling = Parent()->ChildAt(0);
					bool bSameLetterFound = false;
					if (sibling) {
						do {
							SLVpointer = dynamic_cast<SingleLetterView*>(sibling);
							if ((SLVpointer) &&
								(sibling != this) &&
								(SLVpointer->GetCharacter() == lowercase[0]))
							{
								bSameLetterFound = true;
							}
							sibling = sibling->NextSibling();
						} while (!bSameLetterFound && sibling);
					}
					if (!bSameLetterFound)
					{
						BTextView::KeyDown(lowercase, numBytes);
					}
				}
			} else {
				
			}
		}
	}
}

void SingleLetterView::SetActive(bool flag)
{
	// Basically, what I want to do is to draw a thick red line from the
	// upper right corner of the control's frame to the bottom left corner
	if (GetCharacter()) return;
	rgb_color background = ui_color(B_CONTROL_BACKGROUND_COLOR);
	
/*	if (!flag) {
		SetViewBitmap(internalBitmap); // , &background);
	} else {
		ClearViewBitmap();
	}
*/	
	static rgb_color currentHighColor = HighColor();
	static rgb_color transparent = ui_color(B_DOCUMENT_BACKGROUND_COLOR);
	{
		
		SetLineMode(B_ROUND_CAP, B_ROUND_JOIN);
		BRect bounds = Bounds();
		BPoint upperRight(bounds.Width() - 2, 2);
		BPoint bottomLeft(2, bounds.Height() - 2);
		if (!flag) {			// Crossing out the window if it should NOT be active
			SetPenSize(3.0);			// The line is thick
			SetHighColor(255, 0, 0);	// The line is red
		} else {
			SetPenSize(5.0);			// The line is thick
			SetHighColor(transparent);	// The line is transparent
		}
		StrokeLine(upperRight, bottomLeft);
		SetHighColor(currentHighColor);
	}
	
}

char SingleLetterView::GetCharacter()
{
	return (char )this->ByteAt(0);
}

void SingleLetterView::SetEnabled(bool enable) {
	if (enabled == enable)
		return;

	rgb_color textColor = ui_color(B_DOCUMENT_TEXT_COLOR);
	rgb_color viewColor = ui_color(B_DOCUMENT_BACKGROUND_COLOR);
	rgb_color bgColor   = ui_color(B_MENU_BACKGROUND_COLOR);
	BFont font;

	GetFontAndColor(0, &font);

	if (!enable) {
		textColor = disable_color(textColor, ViewColor());
		viewColor = disable_color(ViewColor(), bgColor);
	}

	SetFontAndColor(&font, 0, &textColor);
	SetViewColor(viewColor);
	SetLowColor(viewColor);

	if (Window() != NULL) {
		MakeEditable(enable);
		if (enable)
			SetFlags(Flags() | B_NAVIGABLE);
		else {
			SetFlags(Flags() & ~B_NAVIGABLE);
			MakeFocus(enable);
		}
		Invalidate();
		Window()->UpdateIfNeeded();
	}
	
	SetActive(GetCharacter() != 0);
	
	enabled = enable;
}

void SingleLetterView::Draw(BRect updateRect)
{
	rgb_color textColor = ui_color(B_DOCUMENT_TEXT_COLOR);
	SetHighColor(textColor);
	BTextView::Draw(updateRect);
	if (!IsFocus())
		SetActive(this->GetCharacter() != 0);
}
