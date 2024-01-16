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
    
    
	DirectKeysBox* directBox = new DirectKeysBox("Box");
	layoutItem = externalGroup->AddView(directBox);
	layoutItem->SetExplicitAlignment(BAlignment(B_FOLLOW_LEFT_RIGHT, B_FOLLOW_TOP_BOTTOM));
	
/*    											
	BGridLayout* directKeysLayout = new BGridLayout();
	externalGroup->AddItem(directKeysLayout);
	externalGroup->SetItemWeight(1, 0);
	
	
	
	BStringView* directKeysLabel = new BStringView("Direct Keys Label", 
		B_TRANSLATE("Direct Keys:"));
	BStringView* ĉLabel = new BStringView("ĉ Label", B_TRANSLATE("ĉ"));
	BStringView* ĝLabel = new BStringView("ĝ Label", B_TRANSLATE("ĝ"));
	BStringView* ĥLabel = new BStringView("ĥ Label", B_TRANSLATE("ĥ"));
	BStringView* ĵLabel = new BStringView("ĵ Label", B_TRANSLATE("ĵ"));
	BStringView* ŝLabel = new BStringView("ŝ Label", B_TRANSLATE("ŝ"));
	BStringView* ŭLabel = new BStringView("ŭ Label", B_TRANSLATE("ŭ"));
	
	BRect glyphSize;
	
	
	SingleLetterView* ĉKey = new SingleLetterView("ĉ Substitute");
	SingleLetterView* ĝKey = new SingleLetterView("ĝ Substitute");
	SingleLetterView* ĵKey = new SingleLetterView("ĵ Substitute");
	SingleLetterView* ĥKey = new SingleLetterView("ĥ Substitute");
	SingleLetterView* ŝKey = new SingleLetterView("ŝ Substitute");
	SingleLetterView* ŭKey = new SingleLetterView("ŭ Substitute");
	ĉKey->GetPreferredSize(&glyphSize);
	BSize preferredSize(glyphSize.Width(), glyphSize.Height());
	
	directKeysLayout->SetMaxRowHeight(0, glyphSize.Height());
	directKeysLayout->SetMaxRowHeight(1, glyphSize.Height() + 5);
	directKeysLayout->SetRowWeight(0, 0);
	directKeysLayout->SetRowWeight(1, 0);
	
	layoutItem = directKeysLayout->AddView(ĉLabel, 1, 0);
	layoutItem->SetExplicitAlignment(BAlignment(B_ALIGN_HORIZONTAL_CENTER,
    											B_ALIGN_MIDDLE));
	layoutItem = directKeysLayout->AddView(ĉKey, 1, 1);
	layoutItem->SetExplicitMinSize(preferredSize);
	layoutItem->SetExplicitMaxSize(preferredSize);
	layoutItem->SetExplicitPreferredSize(preferredSize);
	layoutItem->SetExplicitAlignment(BAlignment(B_ALIGN_HORIZONTAL_CENTER,
    											B_ALIGN_MIDDLE));
	layoutItem = directKeysLayout->AddView(ĝLabel, 2, 0);
	layoutItem->SetExplicitAlignment(BAlignment(B_ALIGN_HORIZONTAL_CENTER,
    											B_ALIGN_MIDDLE));
	layoutItem = directKeysLayout->AddView(ĝKey, 2, 1);
	layoutItem->SetExplicitMinSize(preferredSize);
	layoutItem->SetExplicitMaxSize(preferredSize);
	layoutItem->SetExplicitPreferredSize(preferredSize);
	layoutItem->SetExplicitAlignment(BAlignment(B_ALIGN_HORIZONTAL_CENTER,
    											B_ALIGN_MIDDLE));
	layoutItem = directKeysLayout->AddView(ĥLabel, 3, 0);
	layoutItem->SetExplicitAlignment(BAlignment(B_ALIGN_HORIZONTAL_CENTER,
    											B_ALIGN_MIDDLE));
	layoutItem = directKeysLayout->AddView(ĥKey, 3, 1);
	layoutItem->SetExplicitMinSize(preferredSize);
	layoutItem->SetExplicitMaxSize(preferredSize);
	layoutItem->SetExplicitPreferredSize(preferredSize);
	layoutItem->SetExplicitAlignment(BAlignment(B_ALIGN_HORIZONTAL_CENTER,
    											B_ALIGN_MIDDLE));
	layoutItem = directKeysLayout->AddView(ĵLabel, 4, 0);
	layoutItem->SetExplicitAlignment(BAlignment(B_ALIGN_HORIZONTAL_CENTER,
    											B_ALIGN_MIDDLE));
	layoutItem = directKeysLayout->AddView(ĵKey, 4, 1);
	layoutItem->SetExplicitMinSize(preferredSize);
	layoutItem->SetExplicitMaxSize(preferredSize);
	layoutItem->SetExplicitPreferredSize(preferredSize);
	layoutItem->SetExplicitAlignment(BAlignment(B_ALIGN_HORIZONTAL_CENTER,
    											B_ALIGN_MIDDLE));
	layoutItem = directKeysLayout->AddView(ŝLabel, 5, 0);
	layoutItem->SetExplicitAlignment(BAlignment(B_ALIGN_HORIZONTAL_CENTER,
    											B_ALIGN_MIDDLE));
	layoutItem = directKeysLayout->AddView(ŝKey, 5, 1);
	layoutItem->SetExplicitMinSize(preferredSize);
	layoutItem->SetExplicitMaxSize(preferredSize);
	layoutItem->SetExplicitPreferredSize(preferredSize);
	layoutItem->SetExplicitAlignment(BAlignment(B_ALIGN_HORIZONTAL_CENTER,
    											B_ALIGN_MIDDLE));
	layoutItem = directKeysLayout->AddView(ŭLabel, 6, 0);
	layoutItem->SetExplicitAlignment(BAlignment(B_ALIGN_HORIZONTAL_CENTER,
    											B_ALIGN_MIDDLE));
	layoutItem = directKeysLayout->AddView(ŭKey, 6, 1);
	layoutItem->SetExplicitMinSize(preferredSize);
	layoutItem->SetExplicitMaxSize(preferredSize);
	layoutItem->SetExplicitPreferredSize(preferredSize);
	layoutItem->SetExplicitAlignment(BAlignment(B_ALIGN_HORIZONTAL_CENTER,
    											B_ALIGN_MIDDLE));
		
	
	directKeysLayout->AddView(directKeysLabel, 0, 1);
*/	
	
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
