/*
 * Copyright 2024, Alexey Burshtein <ahitech@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef DIRECT_KEYS_BOX_H
#define DIRECT_KEYS_BOX_H

#include <Box.h>
#include <CheckBox.h>
#include <SupportDefs.h>

const uint32	BCHECKBOX_TOGGLED = 'BcbT';

class DirectKeysBox : public BBox
{
public:
	DirectKeysBox(const char* name);
	virtual ~DirectKeysBox();
	
	virtual void MessageReceived(BMessage* in);

protected:
	BCheckBox* label;

};


#endif // DIRECT_KEYS_BOX_H
