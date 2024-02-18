#include <Layout.h>
#include <Size.h>
#include "Application.h"

MainApp::MainApp ()
	: BApplication ("application/x-vnd.hitech.EsperantoFilterSettings")
{

}


MainApp::~MainApp ()
{

}

void MainApp::ReadyToRun() {
    window = new AppWindow();
    window->Show();
    BLayout* layout = window->GetLayout();
    if (layout) {
    	BSize size = layout->PreferredSize();
    	window->ResizeTo(size.Width(), size.Height());
    }
}




int main()
{
	MainApp *app = new MainApp;
   /* Further initialization goes here -- read settings,
      set globals, etc. */
   app->Run();
   /* Clean up -- write settings, etc. */
   delete be_app;
   return 0;
}
