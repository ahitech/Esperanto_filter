/*
 * Copyright 2024, Alex Hitech <ahitech@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#include <Catalog.h>
#include <GridLayout.h>
#include <GroupLayout.h>
#include <Layout.h>
#include <StringView.h>
#include <TextControl.h>


#include "MainView.h"


#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "Main View"

MainView::MainView (BRect frame)
	: BView (frame, "main View",
			 B_FOLLOW_ALL_SIDES,
			 B_WILL_DRAW | B_NAVIGABLE)
{
	this->SetViewColor (ui_color(B_MENU_BACKGROUND_COLOR));
	
	BGroupLayout* externalGroup = new BGroupLayout(B_VERTICAL);
	externalGroup->SetInsets(10, 10);
	
	this->SetLayout(externalGroup);
	
	BTextControl* postfixSymbols = new BTextControl("Postfix", 
									B_TRANSLATE("Postfix symbols:"), "^", NULL);
	postfixSymbols->ResizeToPreferred();
	
	BLayoutItem* layoutItem = externalGroup->AddView(postfixSymbols);
	layoutItem->SetExplicitAlignment(BAlignment(B_ALIGN_HORIZONTAL_CENTER,
    											B_ALIGN_TOP));
    
    
    											
	BGridLayout* directKeysLayout = new BGridLayout();
	externalGroup->AddItem(directKeysLayout);
	
	BStringView* directKeysLabel = new BStringView("Direct Keys Label", 
		B_TRANSLATE("Direct Keys:"));
	BStringView* ĉLabel = new BStringView("ĉ Label", B_TRANSLATE("ĉ"));
	BStringView* ĝLabel = new BStringView("ĝ Label", B_TRANSLATE("ĝ"));
	BStringView* ĥLabel = new BStringView("ĥ Label", B_TRANSLATE("ĥ"));
	BStringView* ĵLabel = new BStringView("ĵ Label", B_TRANSLATE("ĵ"));
	BStringView* ŝLabel = new BStringView("ŝ Label", B_TRANSLATE("ŝ"));
	BStringView* ŭLabel = new BStringView("ŭ Label", B_TRANSLATE("ŭ"));
	BTextControl* ĉKey = new BTextControl("ĉ Substitute", "", " ", NULL);
	BTextControl* ĝKey = new BTextControl("ĝ Substitute", "", " ", NULL);
	BTextControl* ĵKey = new BTextControl("ĵ Substitute", "", " ", NULL);
	BTextControl* ĥKey = new BTextControl("ĥ Substitute", "", " ", NULL);
	BTextControl* ŝKey = new BTextControl("ŝ Substitute", "", " ", NULL);
	BTextControl* ŭKey = new BTextControl("ŭ Substitute", "", " ", NULL);
	
	directKeysLayout->AddView(ĉLabel, 1, 0);
	directKeysLayout->AddView(ĉKey, 1, 1);
	directKeysLayout->AddView(ĝLabel, 2, 0);
	directKeysLayout->AddView(ĝKey, 2, 1);
	directKeysLayout->AddView(ĥLabel, 3, 0);
	directKeysLayout->AddView(ĥKey, 3, 1);
	directKeysLayout->AddView(ĵLabel, 4, 0);
	directKeysLayout->AddView(ĵKey, 4, 1);
	directKeysLayout->AddView(ŝLabel, 5, 0);
	directKeysLayout->AddView(ŝKey, 5, 1);
	directKeysLayout->AddView(ŭLabel, 6, 0);
	directKeysLayout->AddView(ŭKey, 6, 1);
	
		
	
	directKeysLayout->AddView(directKeysLabel, 0, 1);
	
	
}


MainView::~MainView ()
{
	
}



void MainView::AttachedToWindow() 
{
   if ( Parent() )
      SetViewColor(Parent()->ViewColor());
   BView::AttachedToWindow();
}
