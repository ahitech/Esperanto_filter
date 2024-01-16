/*
 * Copyright 2024, Alexey Burshtein <ahitech@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */

#include <Catalog.h>
#include <GridLayout.h>
#include <GroupLayout.h>
#include <Layout.h>
#include <LayoutBuilder.h>
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
	
	BStringView* directKeysLabel = new BStringView(Bounds(), "Direct Keys Label", 
		B_TRANSLATE("Direct Keys:"));
	AddChild(directKeysLabel);
			
}


DirectKeysBox::~DirectKeysBox()
{
	
}


void DirectKeysBox::AttachedToWindow()
{
	BBox::AttachedToWindow();

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
