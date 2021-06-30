﻿// Fill out your copyright notice in the Description page of Project Settings.

#include "TPError.h"
#include "Logging/LogMacros.h"

void TPError::PrintError(const wchar_t* const message)
{
	UE_LOG(LogTemp, Error, TEXT("Error:%s"), message);
}

void TPError::PrintError(const wchar_t* const message, const int code)
{	
	auto cated = ConcatMessage(message, code);
	PrintError(cated);
	delete cated;
}

const wchar_t* TPError::ConcatMessage(const wchar_t* const message, const int code)
{
	wchar_t errorCode[10];
	auto codeBufSize = sizeof(errorCode) / sizeof(wchar_t);
	auto errMsgBufSize = wcslen(message) + codeBufSize;
	wchar_t* errorMsg = new wchar_t[errMsgBufSize];
	wcscpy_s(errorMsg, errMsgBufSize, message);
	swprintf_s(errorCode, codeBufSize, L":%d", code);
	wcscat_s(errorMsg, errMsgBufSize, errorCode);
	return errorMsg;
}