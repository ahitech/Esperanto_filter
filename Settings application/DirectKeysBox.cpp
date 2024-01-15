/*
 * Copyright 2024, Alexey Burshtein <ahitech@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "DirectKeysBox.h"

DirectKeysBox::DirectKeysBox(const char* name)
	: BBox(name),
	  label(NULL)
{
	BMessage* toBeSent = new BMessage(BCHECKBOX_TOGGLED);
	
	label = new BCheckBox("BBox label",
						  B_TRANSLATE("Use direct keys"),
						  toBeSent);
	
	
}


DirectKeysBox::~DirectKeysBox()
{
	
}


void DirectKeysBox::MessageReceived(BMessage *in)
{
	
}
