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

#ifdef		DEBUG
#define		TRACE(text)		{ FILE* logA = fopen("/boot/home/Log.txt", "at"); if (!logA) { logA = fopen("/boot/home/Log.txt", "wt"); } if (logA) { fprintf(logA, text); fflush(logA); fclose(logA); } }
#else
#define		TRACE(text)
#endif

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
	virtual status_t		LoadMessage(const char* , const BResources*, BMessage**);
	virtual status_t		PopulateMap(void);
	
	bool					fInitScrolling;
	BPoint					fCurrentMousePosition;
	BMessage*				fSettings;

	BMessage				*pMessagec,
							*pMessageC,
							*pMessageg,
							*pMessageG,
							*pMessageh,
							*pMessageH,
							*pMessagej,
							*pMessageJ,
							*pMessages,
							*pMessageS,
							*pMessageu,
							*pMessageU,
							*pMessageB;
	BFile					*pLibraryFile;
//	std::map<char, BMessage* > mMessagesMap;	
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
	: pMessagec(NULL),
	  pMessageC(NULL),
	  pMessageg(NULL),
	  pMessageG(NULL),
	  pMessageh(NULL),
	  pMessageH(NULL),
	  pMessagej(NULL),
	  pMessageJ(NULL),
	  pMessages(NULL),
	  pMessageS(NULL),
	  pMessageu(NULL),
	  pMessageU(NULL),
	  pMessageB(NULL),
	  pLibraryFile(NULL)
{
	TRACE("-------------=====< INITIALIZATION >=====---------------\n");
	this->fSettings = new BMessage (SETTINGS_MESSAGE_CONSTANT);
	
	PopulateMap();
//	GetSettings();

}

EsperantoInputFilter::~EsperantoInputFilter()
{
	if (pMessagec) { delete pMessagec; pMessagec = NULL; }
	if (pMessageC) { delete pMessageC; pMessageC = NULL; }
	if (pMessageg) { delete pMessageg; pMessageg = NULL; }
	if (pMessageG) { delete pMessageG; pMessageG = NULL; }
	if (pMessageh) { delete pMessageh; pMessageh = NULL; }
	if (pMessageH) { delete pMessageH; pMessageH = NULL; }
	if (pMessagej) { delete pMessagej; pMessagej = NULL; }
	if (pMessageJ) { delete pMessageJ; pMessageJ = NULL; }
	if (pMessages) { delete pMessages; pMessages = NULL; }
	if (pMessageS) { delete pMessageS; pMessageS = NULL; }
	if (pMessageu) { delete pMessageu; pMessageu = NULL; }
	if (pMessageU) { delete pMessageU; pMessageU = NULL; }
	if (pMessageB) { delete pMessageB; pMessageB = NULL; }
	if (pLibraryFile) { delete pLibraryFile; pLibraryFile = NULL; }
	TRACE("-------------=====< SHUTDOWN >=====---------------\n");
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
	BResources *toReturnRes = new BResources();
	BPath pathToCheck;
	if (!pLibraryFile) { pLibraryFile = new BFile(); }
	
	status_t status = B_ENTRY_NOT_FOUND;
	
	// First - check the Haiku add-ons directory
	if (status != B_OK)
	{
		status = find_directory(B_SYSTEM_ADDONS_DIRECTORY, &pathToCheck);
		
		if (status == B_OK) {
			pathToCheck.Append("input_server/filters/EsperantoFilter");
			status = pLibraryFile->SetTo(pathToCheck.Path(), B_READ_ONLY);
		}
	}
	
	// Second - check the Haiku non-packaged add-ons directory
	if (status != B_OK)
	{
		status = find_directory(B_SYSTEM_NONPACKAGED_ADDONS_DIRECTORY, &pathToCheck);
		
		if (status == B_OK) {
			pathToCheck.Append("input_server/filters/EsperantoFilter");
			status = pLibraryFile->SetTo(pathToCheck.Path(), B_READ_ONLY);
		}
	}
	
	// Third - check the user's add-ons directory
	if (status != B_OK)
	{
		status = find_directory(B_USER_ADDONS_DIRECTORY, &pathToCheck);
		
		if (status == B_OK) {
			pathToCheck.Append("input_server/filters/EsperantoFilter");
			status = pLibraryFile->SetTo(pathToCheck.Path(), B_READ_ONLY);
		}
	}
	
	// Fourth - check the user's non-packaged add-ons directory
	if (status != B_OK)
	{
		status = find_directory(B_USER_NONPACKAGED_ADDONS_DIRECTORY, &pathToCheck);
		
		if (status == B_OK) {
			pathToCheck.Append("input_server/filters/EsperantoFilter");
			status = pLibraryFile->SetTo(pathToCheck.Path(), B_READ_ONLY);
		}
	}
	
	// It's really strange that we arrived here, since this code has to be
	// executed somehow, but the library can't be found anywhere. Well, let's report
	// an error.
	if (status != B_OK) {
		return (NULL);
	}
	
	// File of the library is found. Let's check its resources:
	if (toReturnRes) {
		status = toReturnRes->SetTo(pLibraryFile);
		if (status != B_OK) {
			return NULL;
		}
	}
	return toReturnRes;
}



/**
 *	\brief		Load message into one of the parameters
 *	\param	resourceName[IN]	Name of the resource to load
 *	\param	pLibraryFile[IN]	Pointer to the resources file
 *	\param	out[OUT]			Pointer to the pointer to unflattened message.
 *								This pointer can't be NULL. But the adressed
 *								message can be NULL (and, as a matter of fact, expected).
 *								The actual message is allocated within the function.
 *	\return		B_OK	If succeeded to unflatten the message
 *				B_NO_INIT	If one of the input parameters are NULL
 *				B_NAME_NOT_FOUND	If the specified message was not found
 *	\note		Caller is responsible for clearing the allocated memory!
 */
status_t EsperantoInputFilter::LoadMessage( const char* resourceName,
											const BResources* pLibraryFile,
											BMessage** out)
{
	status_t toReturn = B_OK;
	size_t	outSize = 0;
	if (!out || !resourceName || !pLibraryFile) { return B_NO_INIT; }
	const void* loadedData;
	
	TRACE(resourceName); TRACE("\n");
	
	if (pLibraryFile->HasResource(B_MESSAGE_TYPE, resourceName)) {
		
		// Loading the message into memory
		loadedData = (BMessage*)pLibraryFile->LoadResource(B_MESSAGE_TYPE, resourceName, &outSize);
		if (!loadedData) {
			toReturn = B_NAME_NOT_FOUND;
		} else {
			TRACE("Loaded the resource successfully!\n");
			toReturn = B_OK;
		}
		
		// Unflatten the loaded data
		if (!(*out)) { *out = new BMessage(); }
		if (*out) { 	// If allocation failed, I don't want to crash Input_server.
			(*out)->Unflatten((const char*)loadedData);
			TRACE("Unflattened the message\n");
			if ((*out)->what == '_KYD')
			{
				TRACE("The message is KeyDown message.\n");	
			} else {
				TRACE("The message is something problematic.\n");
			}
		}
	} else {
		toReturn = B_NAME_NOT_FOUND;
		*out = NULL;
		TRACE("Didn't succeed to unflatten the message.\n");
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
	
	pLibraryFile->PreloadResourceType(B_MESSAGE_TYPE);
	
	BMessage* msg = NULL;
	
	// ĉ
	if (toReturn == B_OK) {
		toReturn = this->LoadMessage("lowercaseC", pLibraryFile, &pMessagec);
		if (toReturn == B_OK) {
			TRACE("Added message for \'c\' successfully!\n");
		}
	}
	
	// Ĉ
	if (toReturn == B_OK) {
		toReturn = this->LoadMessage("uppercaseC", pLibraryFile, &pMessageC);
		if (toReturn == B_OK) {
			TRACE("Added message for \'C\' successfully!\n");
		}
	}

	// ĝ	
	if (toReturn == B_OK) {
		toReturn = this->LoadMessage("lowercaseG", pLibraryFile, &pMessageg);
		if (toReturn == B_OK) {
			TRACE("Added message for \'g\' successfully!\n");
		}
	}
	
	// Ĝ
	if (toReturn == B_OK) {
		toReturn = this->LoadMessage("uppercaseG", pLibraryFile, &pMessageG);
		if (toReturn == B_OK) {
			TRACE("Added message for \'G\' successfully!\n");
		}
	}
	
	// ĥ
	if (toReturn == B_OK) {
		toReturn = this->LoadMessage("lowercaseH", pLibraryFile, &pMessageh);
		if (toReturn == B_OK) {
			TRACE("Added message for \'h\' successfully!\n");
		}
	}
	
	// Ĥ
	if (toReturn == B_OK) {
		toReturn = this->LoadMessage("uppercaseH", pLibraryFile, &pMessageH);
		if (toReturn == B_OK) {
			TRACE("Added message for \'H\' successfully!\n");
		}
	}
		
	// ĵ
	if (toReturn == B_OK) {
		toReturn = this->LoadMessage("lowercaseJ", pLibraryFile, &pMessagej);
		if (toReturn == B_OK) {
			TRACE("Added message for \'j\' successfully!\n");
		}
	}
	
	// Ĵ
	if (toReturn == B_OK) {
		toReturn = this->LoadMessage("uppercaseJ", pLibraryFile, &pMessageJ);
		if (toReturn == B_OK) {
			TRACE("Added message for \'J\' successfully!\n");
		}
	}
	
	// ŝ
	if (toReturn == B_OK) {
		toReturn = this->LoadMessage("lowercaseS", pLibraryFile, &pMessages);
		if (toReturn == B_OK) {
			TRACE("Added message for \'s\' successfully!\n");
		}
	}
	
	// Ŝ
	if (toReturn == B_OK) {
		toReturn = this->LoadMessage("uppercaseS", pLibraryFile, &pMessageS);
		if (toReturn == B_OK) {
			TRACE("Added message for \'S\' successfully!\n");
		}
	}
	
	// ŭ
	if (toReturn == B_OK) {
		toReturn = this->LoadMessage("lowercaseU", pLibraryFile, &pMessageu);
		if (toReturn == B_OK) {
			TRACE("Added message for \'u\' successfully!\n");
		}
	}
	
	// Ŭ
	if (toReturn == B_OK) {
		toReturn = this->LoadMessage("uppercaseU", pLibraryFile, &pMessageU);
		if (toReturn == B_OK) {
			TRACE("Added message for \'U\' successfully!\n");
		}
	}
	
	// Backspace
	if (toReturn == B_OK) {
		toReturn = this->LoadMessage("Backspace", pLibraryFile, &pMessageB);
		if (toReturn == B_OK) {
			TRACE("Added message for Backspace successfully!\n");
		}
		if (!pMessageB) {
			TRACE("Message for Backspace is NULL!\n");
		} else if (pMessageB->what != '_KYD') {
			TRACE("Message for Backspace is not KeyDown!\n");
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
	static BMessage 	sMessage;
	static int32 modifiers;
	filter_result	toReturn = B_DISPATCH_MESSAGE;
	char   buffer[100];
	
	if (!in)		// Sanity check
	{
		return B_SKIP_MESSAGE;
	}
	
	const char* bytes;
	if (in->FindString("bytes", &bytes) != B_OK)
	{
		return toReturn;
	}

	if (in->FindInt32("modifiers", &modifiers) != B_OK)
	{
		return toReturn;
	}
	
	if (in->what == B_KEY_DOWN)
	{
		switch (eCurrentState)
		{
			case 	STATE_0_NORMAL_WORK:
			{
				TRACE("Normal work.\n");
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
					{
						TRACE("Found interesting character!\n");
						cSavedCharacter = bytes[0];
						sprintf(buffer, "The character is %c.\n", cSavedCharacter);
						TRACE(buffer);
						sMessage = *in;	// This duplicates the message with the interesting character
						toReturn = B_DISPATCH_MESSAGE;
						break;
					}
					default:
						toReturn = B_DISPATCH_MESSAGE;
						break;
				};		// <-- end of "switch (interesting letter)"
				eCurrentState = STATE_1_CHARACTER_RECEIVED;
				break;
			}
			
			case 	STATE_1_CHARACTER_RECEIVED:
			{
				switch (bytes[0]) 
				{
					case '^':
					{
						TRACE("Found caret!\n");
						
						BMessage *pmBackSpace = new BMessage(), 
								 *pmNewCharacter;
						*pmBackSpace = *pMessageB;		// Duplicating the backspace message
						switch (cSavedCharacter) {
							case 'c': { pmNewCharacter = new BMessage(*pMessagec); break; }
							case 'C': { pmNewCharacter = new BMessage(*pMessageC); break; }
							case 'g': { pmNewCharacter = new BMessage(*pMessageg); break; }
							case 'G': { pmNewCharacter = new BMessage(*pMessageG); break; }
							case 'h': { pmNewCharacter = new BMessage(*pMessageh); break; }
							case 'H': { pmNewCharacter = new BMessage(*pMessageH); break; }
							case 'j': { pmNewCharacter = new BMessage(*pMessagej); break; }
							case 'J': { pmNewCharacter = new BMessage(*pMessageJ); break; }
							case 's': { pmNewCharacter = new BMessage(*pMessages); break; }
							case 'S': { pmNewCharacter = new BMessage(*pMessageS); break; }
							case 'u': { pmNewCharacter = new BMessage(*pMessageu); break; }
							case 'U': { pmNewCharacter = new BMessage(*pMessageU); break; }
							default:  { pmNewCharacter = NULL; break; }
						}
						if (!pmNewCharacter) {
							TRACE("Did not find the message for the new character.\n");
						}
						if (!pmBackSpace) { 
							TRACE("Did not find the message for the Backspace! Bailing out...\n");
						}
						if (!pmNewCharacter || !pmBackSpace) {
							toReturn = B_DISPATCH_MESSAGE; 
							break; 
						}
						
						TRACE("Found both Backspace and new character messages.\n");
						
						TRACE("Going to replace the time in Backspace message...\n");
						pmBackSpace->ReplaceInt64("when", real_time_clock_usecs());
						//	pmBackSpace->ReplaceInt32("modifiers", modifiers);	<-- Are modifiers necessary?
 						outList->AddItem(pmBackSpace);
 						
 						TRACE("Adding Backspace to the list of messages to send.\n");
 						
 						pmNewCharacter->ReplaceInt64("when", real_time_clock_usecs());
 						pmNewCharacter->ReplaceInt32("modifiers", modifiers);	// Here probably they are.
 						outList->AddItem(pmNewCharacter);
 						TRACE("Adding the character with circumflex to the list of messages to send.\n");
 						
						eCurrentState = STATE_2_CARET_RECEIVED;
						toReturn = B_DISPATCH_MESSAGE;
						break;
					}
					default:
					{
						// The user has entered a character which is not a caret.
						// We need to reset all expectations and start anew.
						eCurrentState = STATE_0_NORMAL_WORK;
						sMessage.MakeEmpty();
						cSavedCharacter = '0';
						
						// Probably the current character was one of those which may have diactrics
						toReturn = this->Filter(in, outList);
						break;
					}		
				}	
				break;	// <-- end of "case (the second entered character)"
			}
			case STATE_2_CARET_RECEIVED:
			{
				switch (bytes[0])
				{
					case '^':	// Second caret after a caret removes diactrics and prints 
								// the original character and a single caret
					{
						if (true)	//	<-- This should be changed to a setting
						{
							TRACE("Second caret received!\n");
							
							BMessage *pmBackSpace = new BMessage(),
									 *pmNewCharacter = new BMessage(),
									 *pmCaret = new BMessage();
							*pmBackSpace = *pMessageB;
							*pmNewCharacter = sMessage;
							*pmCaret = *in;
						
							// Sending Backspace
							pmBackSpace->ReplaceInt64("when", real_time_clock_usecs());
							//	pmBackSpace->ReplaceInt32("modifiers", modifiers);	<-- Are modifiers necessary?
	 						outList->AddItem(pmBackSpace);
	 						
	 						TRACE("Added Backspace to list of messages to be sent successfully.\n");
	 						
	 						// Sending the original character
	 						pmNewCharacter->ReplaceInt64("when", real_time_clock_usecs());
	 						// pmNewCharacter->ReplaceInt32("modifiers", modifiers); <-- The modifiers are original
	 						outList->AddItem(pmNewCharacter);
	 						
	 						TRACE("Added original message with the original character successfully.\n");
	 						
	 						// Sending a single caret
	 						pmCaret->ReplaceInt64("when", real_time_clock_usecs());
	 						outList->AddItem(pmCaret);
	 						
	 						TRACE("Added caret successfully.\n");
	 						
	 						// Returning to normal work
							eCurrentState = STATE_0_NORMAL_WORK;
							sMessage.MakeEmpty();
							cSavedCharacter = '0';

							toReturn = B_DISPATCH_MESSAGE;
						}
						break;
					}	// <-- end of "case (two carets received)"
					default:
					{
						// The user has entered a character which is not a caret.
						// We need to reset all expectations and start anew.
						eCurrentState = STATE_0_NORMAL_WORK;
						sMessage.MakeEmpty();
						cSavedCharacter = '0';
						
						// Probably the current character was one of those which may have diactrics
						toReturn = this->Filter(in, outList);
						break;
					}
				}	// <-- end of "switch (character after caret)"
				break;
			}	// <-- end of "case (next charater after received caret)"
			default:
				toReturn = B_DISPATCH_MESSAGE;
		}	// <-- end of "switch (current state of the state machine)"
	}	// <-- end of "if (a key was pressed)"
	
	return toReturn;
}

extern "C" BInputServerFilter* instantiate_input_filter() {
	return new(std::nothrow) EsperantoInputFilter();
}
