/*
 * Copyright 2024, Alex Hitech <ahitech@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */

#include <Catalog.h>
#include <GridLayout.h>
#include <GroupLayout.h>
#include <Layout.h>
#include <LayoutBuilder.h>
#include <Messenger.h>
#include <StringView.h>
#include <TextControl.h>


#include "MainView.h"


#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "Main View"

const uint32	BCHECKBOX_TOGGLED 		= 'BcbT';
const uint32	AUTO_STARTUP_TOGGLED 	= 'AsTg';
const uint32	STARTUP_ACTIVE_TOGGLED 	= 'StAc';
const uint32	AUTO_U_AFTER_A_E		= 'AuAE';

MainView::MainView (BRect frame)
	: BView (frame, "main View",
			 B_FOLLOW_ALL_SIDES,
			 B_WILL_DRAW | B_NAVIGABLE),
	  directBox(NULL),
	  startupSettings(NULL),
	  bUseDirectKeys(false),			// TODO: This should be read from the settings
	  bStartupWithTheSystem(false)		// TODO: This should be read from the settings
{
	this->SetViewColor (ui_color(B_MENU_BACKGROUND_COLOR));
	
	BGroupLayout* externalGroup = new BGroupLayout(B_VERTICAL);
	externalGroup->SetInsets(10, 10);
	
	this->SetLayout(externalGroup);
	
	BTextControl* postfixSymbols = new BTextControl("Postfix", 
									B_TRANSLATE("Postfix symbols:"), "^", NULL);
									
	BCheckBox* autoŬ = new BCheckBox("autoU",
		B_TRANSLATE("Automatic ŭ after a, e"),
		new BMessage(AUTO_U_AFTER_A_E));

	postfixSymbols->ResizeToPreferred();
	
	BBox* textManipulation = new BBox(B_FANCY_BORDER,
				BLayoutBuilder::Group<>(B_VERTICAL, 1.0f)
					.SetInsets(10.0f, 4.0f, 5.0f, 4.0f)
					.Add(postfixSymbols)
					.Add(autoŬ)
					.View());
	BStringView* textManipulationLabel = new BStringView("text manipulation label",
					B_TRANSLATE("Notation"));
	textManipulation->SetLabel(textManipulationLabel);
	externalGroup->AddView(textManipulation);
	
	BLayoutItem* layoutItem = NULL;
	
	// Direct keys	
    BStringView* ĉLabel = new BStringView("ĉ Label", B_TRANSLATE("ĉ"));
	BStringView* ĝLabel = new BStringView("ĝ Label", B_TRANSLATE("ĝ"));
	BStringView* ĥLabel = new BStringView("ĥ Label", B_TRANSLATE("ĥ"));
	BStringView* ĵLabel = new BStringView("ĵ Label", B_TRANSLATE("ĵ"));
	BStringView* ŝLabel = new BStringView("ŝ Label", B_TRANSLATE("ŝ"));
	BStringView* ŭLabel = new BStringView("ŭ Label", B_TRANSLATE("ŭ"));
	
	ĉKey = new SingleLetterView("ĉ Substitute"); ĉKey->ResizeToPreferred();
	ĝKey = new SingleLetterView("ĝ Substitute");
	ĵKey = new SingleLetterView("ĵ Substitute");
	ĥKey = new SingleLetterView("ĥ Substitute");
	ŝKey = new SingleLetterView("ŝ Substitute");
	ŭKey = new SingleLetterView("ŭ Substitute");
	
	BStringView* downArrow1 = new BStringView("Down arrow 1", "↓");
	BStringView* downArrow2 = new BStringView("Down arrow 2", "↓");
	BStringView* downArrow3 = new BStringView("Down arrow 3", "↓");
	BStringView* downArrow4 = new BStringView("Down arrow 4", "↓");
	BStringView* downArrow5 = new BStringView("Down arrow 5", "↓");
	BStringView* downArrow6 = new BStringView("Down arrow 6", "↓");
	
    BStringView* substituteKeysLabel = new BStringView("Substitute Keys Label", 
		B_TRANSLATE("Substitute these:"));
	BStringView* directKeysLabel = new BStringView("Direct Keys Label", 
		B_TRANSLATE("With these:"));
	BStringView* caseLabel = new BStringView("Case label",
		B_TRANSLATE("uppercase will\nbe converted\nto uppercase"));
	caseLabel->SetAlignment(B_ALIGN_CENTER);
    
    directBox = new BBox(B_FANCY_BORDER,
		BLayoutBuilder::Grid<>(5.0f, 5.0f)
			.SetInsets(10.0f, 0.0f, 5.0f, 0.0f)
			.Add(substituteKeysLabel, 0, 0)
			.Add(caseLabel, 0, 1)
			.Add(directKeysLabel, 0, 2)
			.Add(ĉLabel, 1, 2)
			.Add(ĝLabel, 2, 2)
			.Add(ĥLabel, 3, 2)
			.Add(ĵLabel, 4, 2)
			.Add(ŝLabel, 5, 2)
			.Add(ŭLabel, 6, 2)
			.Add(downArrow1, 1, 1)
			.Add(downArrow2, 2, 1)
			.Add(downArrow3, 3, 1)
			.Add(downArrow4, 4, 1)
			.Add(downArrow5, 5, 1)
			.Add(downArrow6, 6, 1)
			.Add(ĉKey, 1, 0)
			.Add(ĝKey, 2, 0)
			.Add(ĵKey, 3, 0)
			.Add(ĥKey, 4, 0)
			.Add(ŝKey, 5, 0)
			.Add(ŭKey, 6, 0)
			.SetInsets(10.0f, 6.0f, 10.0f, 10.0f)
			.View());
    BCheckBox *label = new BCheckBox(B_TRANSLATE("Use direct keys"),
						  			 new BMessage(BCHECKBOX_TOGGLED));
						  			 
	// Here the value of the Direct Keys substitution is set according
	// to the settings
	label->SetValue(bUseDirectKeys ? 1 : 0);
	directBox->SetLabel(label);
	layoutItem = externalGroup->AddView(directBox);
	
	BRect glyphSize;
	ĉKey->GetPreferredSize(&glyphSize);
	BSize preferredSize(glyphSize.Width(), glyphSize.Height());
	
	BGridLayout* layout = dynamic_cast <BGridLayout*>(directBox->ChildAt(1)->GetLayout());
	if (layout) {
		for (int32 col = 1; col < layout->CountColumns(); ++col)
		{
			layout->ItemAt(col, 2)->SetExplicitAlignment(
							BAlignment(B_ALIGN_CENTER, B_ALIGN_MIDDLE));
			layout->ItemAt(col, 1)->SetExplicitAlignment(
							BAlignment(B_ALIGN_CENTER, B_ALIGN_MIDDLE));
			layout->ItemAt(col, 0)->SetExplicitMinSize(preferredSize);
			layout->ItemAt(col, 0)->SetExplicitMaxSize(preferredSize);
			layout->ItemAt(col, 0)->SetExplicitPreferredSize(preferredSize);
			layout->SetMaxColumnWidth(col, glyphSize.Width());
			layout->SetColumnWeight(col, 0);
		}
		layout->SetColumnWeight(0, 1);
		layout->SetMaxRowHeight(0, glyphSize.Height());
		layout->SetMaxRowHeight(1, glyphSize.Height());
		layout->SetMaxRowHeight(2, glyphSize.Height());
		layout->InvalidateLayout();
	}
	
	// Startup settings
	BCheckBox* automaticStartup = new BCheckBox(
			B_TRANSLATE("Startup with the system"),
			new BMessage(AUTO_STARTUP_TOGGLED));
	BCheckBox* startActive = new BCheckBox("startActive",
			B_TRANSLATE("Start active"),
			new BMessage(STARTUP_ACTIVE_TOGGLED));
	startupSettings = new BBox(B_FANCY_BORDER,
				BLayoutBuilder::Group<>(B_VERTICAL, 1.0f)
					.SetInsets(10.0f, 4.0f, 5.0f, 4.0f)
					.Add(startActive)
					.View());
	startupSettings->SetLabel(automaticStartup);
	automaticStartup->SetValue(true);
	externalGroup->AddView(startupSettings);
	
	// Explanation string
	BStringView* explanation = new BStringView("explanation",
		B_TRANSLATE("No need to save,\nthe changes are applied immediately"));
	layoutItem = externalGroup->AddView(explanation);
	layoutItem->SetExplicitAlignment(BAlignment(B_ALIGN_CENTER, B_ALIGN_MIDDLE));
	
	// Test area
	BTextControl* testArea = new BTextControl("test area", 
				B_TRANSLATE("Test your Esperanto input:"), "", NULL);
	externalGroup->AddView(testArea);
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
			pointer = dynamic_cast<SingleLetterView*>(layout->ItemAt(col, 0)->View());
			if (pointer) pointer->SetEnabled(flag);
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
		case (AUTO_STARTUP_TOGGLED):
		{
			bool value = (bool)(dynamic_cast<BCheckBox*>(startupSettings->LabelView())->Value());
			BCheckBox* startActive = dynamic_cast<BCheckBox*>(startupSettings->FindView("startActive"));
			if (startActive) {
				startActive->SetEnabled(value);
			}
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
      
   BMessenger messenger(this);
   BCheckBox*	checkBox = NULL;
   
   if (directBox) {
	   checkBox = (BCheckBox*)directBox->LabelView();
	   if (checkBox) checkBox->SetTarget(messenger);
	   EnableDirectKeys(bUseDirectKeys);
   }
   
   if (startupSettings) {
   		checkBox = (BCheckBox*)startupSettings->LabelView();
   		if (checkBox) checkBox->SetTarget(messenger);
   }
   
   checkBox = (BCheckBox*)FindView("autoU");
   if (checkBox != NULL) checkBox->SetTarget(messenger);
   		
   
   BView::AttachedToWindow();
}

