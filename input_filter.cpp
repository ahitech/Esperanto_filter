#include <Alert.h>
#include <File.h>
#include <InputServerFilter.h>
#include <Message.h>
#include <Messenger.h>
#include <String.h>
#include <Window.h>

#include <stdio.h>

#define		PATH_TO_BIG_SEE_FILE	\
							"/boot/home/EsperantoFilter-BigSee.msg"
#define		PATH_TO_BAKSPASE_FILE	\
							"/boot/home/EsperantoFilter-Bakspase.msg"
							
#define		SETTINGS_MESSAGE_CONSTANT	'Sset'

#define		POSTFIX_KEYS_NAME	"Postfix keys"


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
	
	bool					fInitScrolling;
	BPoint					fCurrentMousePosition;
	BMessage*				fSettings;

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
//	GetSettings();

}

EsperantoInputFilter::~EsperantoInputFilter()
{

}

status_t EsperantoInputFilter::InitCheck()
{
    return B_OK;
}

filter_result 
EsperantoInputFilter::Filter(BMessage *in, BList *outList)
{
	static bool	bExpectingThePostfix = false;
	static char cPreviousKey = '0';
	BFile *messageToSend = NULL;
	
	FILE* log = fopen ("/boot/home/Log.txt", "a");
	
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
		int8 val;
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
				cPreviousKey = bytes[0];
				break;
			case '^':
			{
				fprintf (log, "Detected caret ^\n");
				
				if (cPreviousKey == '0') {
					fprintf(log, "The previous letter was not one of the interesting ones.\n");
					
					break;
				}
				BMessage *pmBackSpace = new BMessage(*in), 
						*pmNewCharacter = new BMessage(*in);
						
/*				// Prepare the backspace character
				pmBackSpace->ReplaceInt32("key", 0x1E);
				pmBackSpace->ReplaceInt32("modifiers", 0);
//				pmBackSpace->ReplaceUint8("states", 0);
				pmBackSpace->RemoveName("bytes");
				pmBackSpace->AddString("bytes", "");
				pmBackSpace->ReplaceInt8("byte", 0x08);
				pmBackSpace->ReplaceInt32("raw_char", 0x08);
				
				
*/				

				fprintf(log, "Trying to send the backspace\n");

				messageToSend = new BFile("/boot/home/Projects/Esperanto_filter/Backspace.msg", 	// Try opening the file
							   B_READ_ONLY);
				if ((messageToSend->InitCheck()) == B_OK)
				{
					fprintf(log, "Opened the file successfully!\n");
					if (pmBackSpace->Unflatten(messageToSend) == B_OK)	// Perform actual read
					{
						fprintf(log, "Read the backspace message!\n");
						pmBackSpace->ReplaceInt64("when", real_time_clock_usecs());
						outList->AddItem(pmBackSpace);
						fprintf(log, "Added backspace to the output list!\n");
						messageToSend->Unset();					// Flush and close the file
						delete messageToSend;
						messageToSend = NULL;
					}
					else {
						fprintf(log, "Did not succeed to unflatten the backspace!\n");
					}
				} else {
					fprintf(log, "Did not succeed to open the file with backspace!\n");		
				}
				
				
				
				/* Test */

				switch (cPreviousKey) {
					case 'C':
					{
						cPreviousKey = '0';
						
						fprintf(log, "The previous letter was C\n");
						
						messageToSend = new BFile("/boot/home/Projects/Esperanto_filter/C-circumflex.msg", 	// Try opening the file
							   B_READ_ONLY);
						if ((messageToSend->InitCheck()) == B_OK)
						{
							fprintf(log, "Opened the file with C-circumflex successfully!\n");
							if (B_OK == pmNewCharacter->Unflatten(messageToSend))	// Perform actual read
							{
								fprintf(log, "Read the message with C-circumflex successfully!\n");
								pmNewCharacter->ReplaceInt64("when", real_time_clock_usecs());
								outList->AddItem(pmNewCharacter);
								fprintf(log, "Added the C-circumflex successfully!\n");
								messageToSend->Unset();					// Flush and close the file
								delete messageToSend;
								messageToSend = NULL;
							} else {
								fprintf(log, "Did not succeed to unflatten the C-circumflex message!\n");
							}
						} else {
							fprintf(log, "Did not succeed to open the C-circumflex file!\n");
						}
				
						
						break;
					}
					default:
						cPreviousKey = '0';
						break;			
				};	// <-- end of "switch (previous key)"
				
				fprintf(log, "Before exiting the filter: outList has %d messages.\n", outList->CountItems());

				
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
	
	fclose(log);
	return B_DISPATCH_MESSAGE;
}

extern "C" BInputServerFilter* instantiate_input_filter() {
	return new(std::nothrow) EsperantoInputFilter();
}
