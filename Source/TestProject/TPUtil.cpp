#include "TPUtil.h"
#include <stdio.h>
#include <Windows.h>

void TPUtil::CharToWChar(wchar_t* buffer, size_t bufferSize, const char* const source)
{
	auto len = bufferSize / sizeof(wchar_t);
	swprintf(buffer, len, L"%hs", source);
}

void TPUtil::WCharToChar(char* buffer, size_t bufferSize, const wchar_t* const source)
{
	auto len = bufferSize / sizeof(char);
	sprintf_s(buffer, len, "%ws", source);
}

void TPUtil::WCharToMultiByte(char* buffer, size_t bufferSize, const wchar_t* const source)
{
	auto wBuffer = new wchar_t[bufferSize];
	wcscpy_s(wBuffer, bufferSize, source);
	int len = WideCharToMultiByte(CP_ACP, 0, wBuffer, -1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, wBuffer, -1, buffer, len, NULL, NULL);
	delete[] wBuffer;
}

void TPUtil::MultiByteToWChar(wchar_t* buffer, size_t bufferSize, const char* const source)
{
	auto hBuffer = new char[bufferSize];
	strcpy_s(hBuffer, bufferSize, source);
	int len = MultiByteToWideChar(CP_ACP, 0, hBuffer, bufferSize, NULL, NULL);
	MultiByteToWideChar(CP_ACP, 0, hBuffer, bufferSize, buffer, len);
	delete[] hBuffer;
}
