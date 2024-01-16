/*
 * Copyright 2024, Alexey Burshtein <ahitech@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */

#include <Catalog.h>
#include <GridLayout.h>
#include <GroupLayout.h>
#include <Layout.h>
#include <StringView.h>

#include "DirectKeysBox.h"
#include "SingleLetterView.h"


#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "Direct keys BBox"

DirectKeysBox::DirectKeysBox(const char* name)
	: BBox(name),
	  label(NULL)
{
	BMessage* toBeSent = new BMessage(BCHECKBOX_TOGGLED);
	
	label = new BCheckBox("BBox label",
						  B_TRANSLATE("Use direct keys"),
						  toBeSent);
	SetLabel(label);
	
	BGridLayout* directKeysLayout = new BGridLayout();
//	this->SetLayout(directKeysLayout);
	
	
	
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
	
	BLayoutItem* layoutItem = directKeysLayout->AddView(ĉLabel, 1, 0);
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
	
	this->AddChild(directKeysLayout->View());
}


DirectKeysBox::~DirectKeysBox()
{
	
}


void DirectKeysBox::MessageReceived(BMessage *in)
{
	if (!in) return;
	
	switch (in->what)
	{
		case 	BCHECKBOX_TOGGLED:
		{
			if (label)
			{
				if (label->Value() == 0)		// Unchecked
				{
					// Disable
					
				}
				else {
					// Enable
						
				}
			}
			break;	
		}
	};
	BBox::MessageReceived(in);
}
