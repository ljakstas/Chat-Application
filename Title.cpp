#include "Title.h"
#include <Windows.h>

using namespace ChatApplication; // This is your project name

int main(array<System::String^>^ args)
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	ChatApplication::Title nameObjectForm;
	Application::Run(% nameObjectForm);
	return 0;
}
