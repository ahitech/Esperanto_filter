#include <Alert.h>
#include <File.h>
#include <FindDirectory.h>
#include <InputServerFilter.h>
#include <Message.h>
#include <Messenger.h>
#include <Path.h>
#include <Resources.h>
#include <String.h>
#include <Window.h>

#include <map>
#include <stdio.h>

#define		PATH_TO_BIG_SEE_FILE	\
							"/boot/home/EsperantoFilter-BigSee.msg"
#define		PATH_TO_BAKSPASE_FILE	\
							"/boot/home/EsperantoFilter-Bakspase.msg"
							
#define		SETTINGS_MESSAGE_CONSTANT	'Sset'

#define		POSTFIX_KEYS_NAME	"Postfix keys"

#define		TRACE(text)		{ FILE* logA = fopen("/boot/home/Log.txt", "a"); if (logA) { fprintf(logA, text); fflush(logA); fclose(logA); } }

class EsperantoInputFilter : public BInputServerFilter
{
public:
							EsperantoInputFilter();
	virtual 				~EsperantoInputFilter();
    virtual status_t 		InitCheck();
    virtual filter_result 	Filter(BMessage *message, BList *outList);
protected:
	virtual status_t		ReadSettingsFromFile(void);
	virtual status_t		SaveSettings(void);
	virtual status_t		GetSettings(void);
	virtual status_t		CheckSettings(void);
	
	virtual BResources*		FindLibraryFile(void);
	virtual status_t		LoadMessage(const char* , const BResources*, BMessage*);
	virtual status_t		PopulateMap(void);
	
	bool					fInitScrolling;
	BPoint					fCurrentMousePosition;
	BMessage*				fSettings;

	std::map<char, BMessage* > mMessagesMap;	
};


/*
 * This function tries to open the file with settings at the preset path.
 * If successfully, it unflattens contents of the settings file into a BMessage.
 * It is assumed that the BMessage for settings is allocated. 
 *		It will be erased, but should be allocated!
 */
status_t EsperantoInputFilter::ReadSettingsFromFile()
{
	status_t toReturn = B_OK;
	BFile *readFrom = new BFile(PATH_TO_BAKSPASE_FILE, B_READ_ONLY);
	if ((toReturn = readFrom->InitCheck()) == B_OK)
	{
		toReturn = this->fSettings->Unflatten(readFrom);
		readFrom->Unset();								// Close the file
	} else {
	
	}
	delete readFrom;
	return toReturn;
}


/*
 * This function flattens contents of the settings message into the settings file
 * at the predefined path.
 */
status_t EsperantoInputFilter::SaveSettings ()
{
	status_t	toReturn = B_OK;
	BFile *writeTo = new BFile(PATH_TO_BAKSPASE_FILE, 	// Try opening the file
							   B_WRITE_ONLY | B_CREATE_FILE | B_ERASE_FILE);
	if ((toReturn = writeTo->InitCheck()) == B_OK)
	{
		toReturn = this->fSettings->Flatten (writeTo);	// Perform actual write
		writeTo->Unset();								// Flush and close the file
		delete writeTo;
	}
	return (toReturn);
}


status_t	EsperantoInputFilter::GetSettings(void)
{
	status_t	toReturn = B_OK;
	
	// Read info from the file. Return value is not important.
//	ReadSettingsFromFile();
//	toReturn = CheckSettings();			// Create a default settings set
	
	return (toReturn);		// Returning the result of reading the settings
}


status_t	EsperantoInputFilter::CheckSettings(void)
{
	status_t	toReturn = B_OK;
	type_code 	type = B_ANY_TYPE;
	int32		countFound = 0;
	
	if (B_OK != fSettings->GetInfo(POSTFIX_KEYS_NAME, &type, &countFound))
	{
		fSettings->AddString(POSTFIX_KEYS_NAME, "^");
	}
	
	
	return toReturn;
}


EsperantoInputFilter::EsperantoInputFilter()
{
	this->fSettings = new BMessage (SETTINGS_MESSAGE_CONSTANT);
	
	this->mMessagesMap.clear();
//	GetSettings();

}

EsperantoInputFilter::~EsperantoInputFilter()
{

}


/**
 *	\brief		This function returns the pointer to the resource file of this compiled library.
 *	\detais		It passes over add-on directories one-by-one and tests each of them for the
 *				presence of this input_server filter. If this library is found, it is opened as
 *				a BFile entity, resources are initialized, and this BResoucres object is returned
 *				to the caller. If not, NULL is returned.
 *	\returns	Pointer to BResources file of the library, if it is found.
 *				NULL otherwise.
 *	\note		The caller is responsible for closing the BResources and freeing the used memory!
 */
BResources* EsperantoInputFilter::FindLibraryFile(void)
{
	BFile toReturnFile;
	BResources *toReturnRes = NULL;
	BPath pathToCheck;
	
	status_t status = B_ENTRY_NOT_FOUND;
	
	// First - check the Haiku add-ons directory
	status = find_directory(B_SYSTEM_ADDONS_DIRECTORY, &pathToCheck);
	
	if (status != B_OK) {
		pathToCheck.Append("input_server/filters/EsperantoFilter");
		status = toReturnFile.SetTo(pathToCheck.Path(), B_READ_ONLY);
	}
	
	// Second - check the Haiku non-packaged add-ons directory
	status = find_directory(B_SYSTEM_NONPACKAGED_ADDONS_DIRECTORY, &pathToCheck);
	
	if (status != B_OK) {
		pathToCheck.Append("input_server/filters/EsperantoFilter");
		status = toReturnFile.SetTo(pathToCheck.Path(), B_READ_ONLY);
	}
	
	// Third - check the user's add-ons directory
	status = find_directory(B_USER_ADDONS_DIRECTORY, &pathToCheck);
	
	if (status != B_OK) {
		pathToCheck.Append("input_server/filters/EsperantoFilter");
		status = toReturnFile.SetTo(pathToCheck.Path(), B_READ_ONLY);
	}
	
	// Fourth - check the user's non-packaged add-ons directory
	status = find_directory(B_USER_NONPACKAGED_ADDONS_DIRECTORY, &pathToCheck);
	
	if (status != B_OK) {
		pathToCheck.Append("input_server/filters/EsperantoFilter");
		status = toReturnFile.SetTo(pathToCheck.Path(), B_READ_ONLY);
	}
	
	// It's really strange that we arrived here, since this code has to be
	// executed somehow, but the library can't be found anywhere. Well, let's report
	// an error.
	if (status != B_OK) {
		return (NULL);
	}
	
	// File of the library is found. Let's check its resources:
	status = toReturnRes->SetTo(&toReturnFile);
	if (status != B_OK) {
		return NULL;
	}
	return toReturnRes;
}



/**
 *	\brief		Load message into one of the parameters
 *	\param	resourceName[IN]	Name of the resource to load
 *	\param	pLibraryFile[IN]	Pointer to the resources file
 *	\param	out[OUT]			Pointer to the unflattened message
 *	\return		B_OK	If succeeded to unflatten the message
 *				B_NO_INIT	If one of the input parameters are NULL
 *				B_NAME_NOT_FOUND	If the specified message was not found
 *	\note		Caller is responsible for clearing the allocated memory!
 */
status_t EsperantoInputFilter::LoadMessage( const char* resourceName,
											const BResources* pLibraryFile,
											BMessage* out)
{
	status_t toReturn = B_OK;
	size_t	outSize = 0;
	if (!out || !pLibraryFile) { return B_NO_INIT; }
	
	TRACE(resourceName);
	
	if (pLibraryFile->HasResource(B_MESSAGE_TYPE, resourceName)) {
		out = (BMessage*)pLibraryFile->LoadResource(B_MESSAGE_TYPE, resourceName, &outSize);
		if (!out) {
			toReturn = B_NAME_NOT_FOUND;
		} else {
			toReturn = B_OK;
		}
	} else {
		toReturn = B_NAME_NOT_FOUND;
	}
	
	return toReturn;
}



/**
 *	/brief		This function populates the dictionary of the messages.
 *	/details	It finds this input_server add-on library file (using EsperantoInputFilter::FindLibraryFile() ),
 *				then initializes a BResources object with this file and loads the resources.
 *	/returns	B_OK if everything goes well and all BMessages are found.
 *				B_ENTRY_NOT_FOUND if the input_server filter add-on is not found.
 *				B_NO_INIT if not all required resources were found.
 */ 
status_t EsperantoInputFilter::PopulateMap (void)
{
	status_t toReturn = B_OK;
	
	BResources* pLibraryFile = FindLibraryFile();
	
	if (pLibraryFile == NULL) {
		return B_ENTRY_NOT_FOUND;
	}
	
	BMessage* msg = new BMessage();
	
	// ĉ
	if (toReturn == B_OK) {
		toReturn = this->LoadMessage("lowercaseC", pLibraryFile, msg);
		if (toReturn == B_OK) {
			this->mMessagesMap.insert({'c', msg});
		}
	}
	
	// Ĉ
	if (toReturn == B_OK) {
		toReturn = this->LoadMessage("uppercaseC", pLibraryFile, msg);
		if (toReturn == B_OK) {
			this->mMessagesMap.insert({'C', msg});
		}
	}

	// ĝ	
	if (toReturn == B_OK) {
		toReturn = this->LoadMessage("lowercaseG", pLibraryFile, msg);
		if (toReturn == B_OK) {
			this->mMessagesMap.insert({'g', msg});
		}
	}
	
	// Ĝ
	if (toReturn == B_OK) {
		toReturn = this->LoadMessage("uppercaseG.msg", pLibraryFile, msg);
		if (toReturn == B_OK) {
			this->mMessagesMap.insert({'G', msg});
		}
	}
	
	// ĥ
	if (toReturn == B_OK) {
		toReturn = this->LoadMessage("lowercaseH", pLibraryFile, msg);
		if (toReturn == B_OK) {
			this->mMessagesMap.insert({'h', msg});
		}
	}
	
	// Ĥ
	if (toReturn == B_OK) {
		toReturn = this->LoadMessage("uppercaseH", pLibraryFile, msg);
		if (toReturn == B_OK) {
			this->mMessagesMap.insert({'H', msg});
		}
	}
		
	// ĵ
	if (toReturn == B_OK) {
		toReturn = this->LoadMessage("lowercaseJ", pLibraryFile, msg);
		if (toReturn == B_OK) {
			this->mMessagesMap.insert({'j', msg});
		}
	}
	
	// Ĵ
	if (toReturn == B_OK) {
		toReturn = this->LoadMessage("uppercaseJ", pLibraryFile, msg);
		if (toReturn == B_OK) {
			this->mMessagesMap.insert({'J', msg});
		}
	}
	
	// ŝ
	if (toReturn == B_OK) {
		toReturn = this->LoadMessage("lowercaseS", pLibraryFile, msg);
		if (toReturn == B_OK) {
			this->mMessagesMap.insert({'s', msg});
		}
	}
	
	// Ŝ
	if (toReturn == B_OK) {
		toReturn = this->LoadMessage("uppercaseS", pLibraryFile, msg);
		if (toReturn == B_OK) {
			this->mMessagesMap.insert({'S', msg});
		}
	}
	
	// ŭ
	if (toReturn == B_OK) {
		toReturn = this->LoadMessage("lowercaseU", pLibraryFile, msg);
		if (toReturn == B_OK) {
			this->mMessagesMap.insert({'u', msg});
		}
	}
	
	// Ŭ
	if (toReturn == B_OK) {
		toReturn = this->LoadMessage("uppercaseU", pLibraryFile, msg);
		if (toReturn == B_OK) {
			this->mMessagesMap.insert({'U', msg});
		}
	}
	
	// Backspace
	if (toReturn == B_OK) {
		toReturn = this->LoadMessage("Backspace", pLibraryFile, msg);
		if (toReturn == B_OK) {
			this->mMessagesMap.insert({'B', msg});
		}
	}

	return toReturn;
}


status_t EsperantoInputFilter::InitCheck()
{
    return B_OK;
}


#define		STATE_0_NORMAL_WORK				(1)
#define		STATE_1_CHARACTER_RECEIVED		(2)
#define		STATE_2_CARET_RECEIVED			(3)
#define		STATE_3_SECOND_CARET_RECEIVED	(4)

/**
 *	\brief		Main filtering function
 *	\details	This function implements the state machine of the filter.
 */
filter_result 
EsperantoInputFilter::Filter(BMessage *in, BList *outList)
{
	static bool	bExpectingThePostfix = false;
	static char cSavedCharacter = '0';
	static int	eCurrentState = STATE_0_NORMAL_WORK;

	BFile *messageToSend = NULL;
	
	const char* bytes;
	if (in->FindString("bytes", &bytes) != B_OK)
	{
		return B_DISPATCH_MESSAGE;
	}

	int32 modifiers;
	if (in->FindInt32("modifiers", &modifiers) != B_OK)
	{
		return B_DISPATCH_MESSAGE;
	}
	
	if (in->what == B_KEY_DOWN)
	{
		switch (eCurrentState)
		{
			case 	STATE_0_NORMAL_WORK:
			{
				switch (bytes[0]) {
					case 'C':
					case 'H':
					case 'J':
					case 'U':
					case 'S':
					case 'G':
					case 'c':
					case 'g':
					case 'j':
					case 'h':
					case 's':
					case 'u':
						cSavedCharacter = bytes[0];
						break;
					default:
						break;
				};		// <-- end of "switch (interesting letter)"
				eCurrentState = STATE_1_CHARACTER_RECEIVED;
				break;
			}
			
			case 	STATE_1_CHARACTER_RECEIVED:
			{
				switch (bytes[0]) {
					case '^':
					{
						BMessage *pmBackSpace = this->MessagesMap['B'], 
								 *pmNewCharacter = this->MessagesMap[cSavedCharacter];
								 
						
					}
			
				break;
			}	
			
			
			
		}
		
		
		int8 val;
		
			case '^':
			{
				TRACE("Detected caret ^\n");
				
				if (cTwoKeysBack == '0') {
					TRACE("The previous letter was not one of the interesting ones.\n");
					
					break;
				}
				else if () {
					cTwoKeysBack = cOneKeyBack;
					cOneKeyBack = '^';	
				}
				
				
						
/*				// Prepare the backspace character
				pmBackSpace->ReplaceInt32("key", 0x1E);
				pmBackSpace->ReplaceInt32("modifiers", 0);
//				pmBackSpace->ReplaceUint8("states", 0);
				pmBackSpace->RemoveName("bytes");
				pmBackSpace->AddString("bytes", "");
				pmBackSpace->ReplaceInt8("byte", 0x08);
				pmBackSpace->ReplaceInt32("raw_char", 0x08);
				
				
*/				

				TRACE("Trying to send the backspace\n");

				
				
				
				
				/* Test */

				switch (cPreviousKey) {
					case 'C':
					{
						cPreviousKey = '0';
						
						TRACE("The previous letter was C\n");
						
						
				
						
						break;
					}
					default:
						cPreviousKey = '0';
						break;			
				};	// <-- end of "switch (previous key)"
				
				TRACE("Before exiting the filter: outList has N messages.\n");

				
				break;
			}
			default:
				cPreviousKey = '0';
				break;
		};	// <-- end of "switch (first character of the input string)"
			
/*			BFile *writeTo = new BFile(PATH_TO_BIG_SEE_FILE, 	// Try opening the file
							   B_WRITE_ONLY | B_CREATE_FILE | B_ERASE_FILE);
			if ((writeTo->InitCheck()) == B_OK)
			{
				in->Flatten (writeTo);	// Perform actual write
				writeTo->Unset();					// Flush and close the file
				delete writeTo;
			}
		}
		else if  (bytes[0] == B_BACKSPACE) {
			BFile *writeTo = new BFile(PATH_TO_BAKSPASE_FILE, 	// Try opening the file
							   B_WRITE_ONLY | B_CREATE_FILE | B_ERASE_FILE);
			if ((writeTo->InitCheck()) == B_OK)
			{
				in->Flatten (writeTo);	// Perform actual write
				writeTo->Unset();					// Flush and close the file
				delete writeTo;
			}
*/
	}	// <-- end of "if (a key was pressed)"
	
	return B_DISPATCH_MESSAGE;
}

extern "C" BInputServerFilter* instantiate_input_filter() {
	return new(std::nothrow) EsperantoInputFilter();
}
