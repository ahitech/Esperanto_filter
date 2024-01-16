/*
 * Copyright 2024, Alex Hitech <ahitech@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef MAIN_VIEW_H
#define MAIN_VIEW_H

#include <Box.h>
#include <View.h>

#include <SupportDefs.h>

#include "DirectKeysBox.h"
#include "SingleLetterView.h"


class MainView :
	public BView
{
public:
	MainView (BRect);
	~MainView ();
	
	virtual void AttachedToWindow();
	virtual void MessageReceived(BMessage* in);
protected:
	BBox* directBox;
	
	virtual void EnableDirectKeys(bool flag = false);

};


#endif // SOURCE_VIEW_H
