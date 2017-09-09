#include "..\Header Files\consoleclass.h"

ConsoleClass::ConsoleClass()
{
}

ConsoleClass::ConsoleClass(const ConsoleClass &)
{
}

ConsoleClass::~ConsoleClass()
{
}

bool ConsoleClass::Initialize()
{
	FILE* fp = nullptr;

	//Allocate a console
	AllocConsole();

	//Redirect unbuffered STDOUT to the console
	m_stdOutputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	if (m_stdOutputHandle == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	freopen_s(&fp, "CONOUT$", "w", stdout);
	setvbuf(stdout, NULL, _IONBF, 0);

	//Redirect unbuffered STDIN to the console
	m_stdInputHandle = GetStdHandle(STD_INPUT_HANDLE);
	if (m_stdInputHandle == INVALID_HANDLE_VALUE)
	{
		return false;
	}
	
	freopen_s(&fp, "CONIN$", "r", stdin);
	setvbuf(stdin, NULL, _IONBF, 0);

	//Redirect unbuffered STDERR to the console
	m_stdErrorHandle = GetStdHandle(STD_ERROR_HANDLE);
	if (m_stdErrorHandle == INVALID_HANDLE_VALUE)
	{
		return false;
	}
	
	freopen_s(&fp, "CONOUT$", "w", stderr);
	setvbuf(stderr, NULL, _IONBF, 0);

	//Make cout, wcout, cin, wcin, wcerr, cerr, wclog and clog 
	//point to console as well
	std::ios::sync_with_stdio();

	return true;
}

bool ConsoleClass::Frame(std::string& input)
{
	return InputHandle(input);
}

bool ConsoleClass::InputHandle(std::string& input)
{
	DWORD result;
	char inputBuffer[512] = { 0 };
	INPUT_RECORD buffer[128];
	DWORD read;

	result = WaitForSingleObject(m_stdInputHandle, 0);

	if (result == WAIT_OBJECT_0)
	{
		PeekConsoleInput(m_stdInputHandle, buffer, 128, &read);

		switch (buffer->EventType)
		{
		case FOCUS_EVENT:
			FlushConsoleInputBuffer(m_stdInputHandle);
			break;

		case KEY_EVENT:
			ReadConsole(m_stdInputHandle, inputBuffer, 512, &read, NULL);
			input = inputBuffer;
			FlushConsoleInputBuffer(m_stdInputHandle);
			return true;

		case MOUSE_EVENT:
			FlushConsoleInputBuffer(m_stdInputHandle);
			break;

		case MENU_EVENT:
			FlushConsoleInputBuffer(m_stdInputHandle);
			break;

		default:
			break;
		}
	}

	FlushConsoleInputBuffer(m_stdInputHandle);

	return false;
}
