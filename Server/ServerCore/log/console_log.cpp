#include "pch.h"
#include "console_log.h"

ConsoleLog::ConsoleLog()
{
	_std_out = GetStdHandle(STD_OUTPUT_HANDLE);
	_std_err = GetStdHandle(STD_ERROR_HANDLE);
}

void ConsoleLog::WriteStdOut(const Color color, const wchar_t* format, ...)
{
	if (format == nullptr)
		return;

	SetColor(true, color);

	va_list ap;
	va_start(ap, format);
	vwprintf(format, ap);
	va_end(ap);

	fflush(stdout);

	SetColor(true, Color::kWhite);
}

void ConsoleLog::WriteStdErr(const Color color, const wchar_t* format, ...)
{
	wchar_t buffer[kBufferSize];

	if (format == nullptr)
		return;

	SetColor(false, color);

	va_list ap;
	va_start(ap, format);
	vswprintf_s(buffer, kBufferSize, format, ap);
	va_end(ap);

	fwprintf_s(stderr, buffer);
	fflush(stderr);

	SetColor(false, Color::kWhite);
}

void ConsoleLog::SetColor(const bool std_out, Color color)
{
	static uint16_t colors[]
	{
		0,
		FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
		FOREGROUND_RED | FOREGROUND_INTENSITY,
		FOREGROUND_GREEN | FOREGROUND_INTENSITY,
		FOREGROUND_BLUE | FOREGROUND_INTENSITY,
		FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY
	};

	SetConsoleTextAttribute(std_out ? _std_out : _std_err, colors[static_cast<int32_t>(color)]);
}