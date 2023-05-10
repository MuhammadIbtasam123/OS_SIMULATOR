#include "main.h"

using namespace System;
using namespace System::Windows::Forms;
[STAThreadAttribute]

void main(cli::array<System::String^>^ args) {
	Application::SetCompatibleTextRenderingDefault(false);
	Application::EnableVisualStyles();
	Project1::main form;
	Application::Run(% form);
}


