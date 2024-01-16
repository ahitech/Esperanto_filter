/*
 * Copyright 2024, Alex Hitech <ahitech@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */

#include <Catalog.h>
#include <GridLayout.h>
#include <GroupLayout.h>
#include <Layout.h>
#include <LayoutBuilder.h>
#include <StringView.h>
#include <TextControl.h>


#include "MainView.h"


#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "Main View"

const uint32	BCHECKBOX_TOGGLED = 'BcbT';

MainView::MainView (BRect frame)
	: BView (frame, "main View",
			 B_FOLLOW_ALL_SIDES,
			 B_WILL_DRAW | B_NAVIGABLE),
	  directBox(NULL)
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
    BStringView* ĉLabel = new BStringView("ĉ Label", B_TRANSLATE("ĉ"));
	BStringView* ĝLabel = new BStringView("ĝ Label", B_TRANSLATE("ĝ"));
	BStringView* ĥLabel = new BStringView("ĥ Label", B_TRANSLATE("ĥ"));
	BStringView* ĵLabel = new BStringView("ĵ Label", B_TRANSLATE("ĵ"));
	BStringView* ŝLabel = new BStringView("ŝ Label", B_TRANSLATE("ŝ"));
	BStringView* ŭLabel = new BStringView("ŭ Label", B_TRANSLATE("ŭ"));
	
	SingleLetterView* ĉKey = new SingleLetterView("ĉ Substitute"); ĉKey->ResizeToPreferred();
	SingleLetterView* ĝKey = new SingleLetterView("ĝ Substitute");
	SingleLetterView* ĵKey = new SingleLetterView("ĵ Substitute");
	SingleLetterView* ĥKey = new SingleLetterView("ĥ Substitute");
	SingleLetterView* ŝKey = new SingleLetterView("ŝ Substitute");
	SingleLetterView* ŭKey = new SingleLetterView("ŭ Substitute");
    BStringView* directKeysLabel = new BStringView("Direct Keys Label", 
		B_TRANSLATE("Direct Keys:"));
    
    directBox = new BBox(B_FANCY_BORDER,
		BLayoutBuilder::Grid<>(5.0f, 5.0f)
			.Add(directKeysLabel, 0, 1, 1, 2)
			.Add(ĉLabel, 1, 0)
			.Add(ĝLabel, 2, 0)
			.Add(ĥLabel, 3, 0)
			.Add(ĵLabel, 4, 0)
			.Add(ŝLabel, 5, 0)
			.Add(ŭLabel, 6, 0)
			.Add(ĉKey, 1, 1)
			.Add(ĝKey, 2, 1)
			.Add(ĵKey, 3, 1)
			.Add(ĥKey, 4, 1)
			.Add(ŝKey, 5, 1)
			.Add(ŭKey, 6, 1)
			.SetInsets(10.0f, 6.0f, 10.0f, 10.0f)
			.View());
    BCheckBox *label = new BCheckBox("BBox label",
						  B_TRANSLATE("Use direct keys"),
						  new BMessage(BCHECKBOX_TOGGLED));
	label->SetValue(1);
	directBox->SetLabel(label);
	layoutItem = externalGroup->AddView(directBox);
	
	BRect glyphSize;
	ĉKey->GetPreferredSize(&glyphSize);
	BSize preferredSize(glyphSize.Width(), glyphSize.Height());
	
	BGridLayout* layout = dynamic_cast <BGridLayout*>(directBox->ChildAt(1)->GetLayout());
	if (layout) {
		for (int32 col = 1; col < layout->CountColumns(); ++col)
		{
			layout->ItemAt(col, 0)->SetExplicitAlignment(
							BAlignment(B_ALIGN_CENTER, B_ALIGN_MIDDLE));
			layout->ItemAt(col, 1)->SetExplicitMinSize(preferredSize);
			layout->ItemAt(col, 1)->SetExplicitMaxSize(preferredSize);
			layout->ItemAt(col, 1)->SetExplicitPreferredSize(preferredSize);
			layout->SetMaxColumnWidth(col, glyphSize.Width());
			layout->SetColumnWeight(col, 0);
		}
		layout->SetColumnWeight(0, 1);
		layout->SetMaxRowHeight(0, glyphSize.Height());
		layout->SetMaxRowHeight(1, glyphSize.Height());
		layout->InvalidateLayout();
	}
}


MainView::~MainView ()
{
	
}

void MainView::EnableDirectKeys(bool flag)
{
	if (!directBox) return;		// Sanity check
	
	BGridLayout* layout = dynamic_cast <BGridLayout*>(directBox->ChildAt(1)->GetLayout());
	if (layout) {
		SingleLetterView* pointer = NULL;
		for (int32 col = 1; col < layout->CountColumns(); ++col)
		{
			pointer = dynamic_cast<SingleLetterView*>(layout->ItemAt(col, 1)->View());
			if (pointer) pointer->MakeEditable(flag);
		}
	}
}


void MainView::MessageReceived(BMessage* in)
{
	switch (in->what)
	{
		case (BCHECKBOX_TOGGLED):
		{
			
			bool value = (bool)(dynamic_cast<BCheckBox*>(directBox->LabelView())->Value());
			EnableDirectKeys(value);
			
			break;	
		}
		default:
			break;
	}
	BView::MessageReceived(in);
}

void MainView::AttachedToWindow() 
{
   if ( Parent() )
      SetViewColor(Parent()->ViewColor());
   BView::AttachedToWindow();
}
