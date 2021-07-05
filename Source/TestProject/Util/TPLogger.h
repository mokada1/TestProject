// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TSingleton.h"
#include "Logging/LogMacros.h"

#include <stdio.h>

class TESTPROJECT_API TPLogger : public TSingleton<TPLogger>
{
public:
	template<typename ... Args>
	void PrintLog(const char* const format, Args ... args)
	{
		const auto bufferSize = snprintf(nullptr, 0, format, args ...) + 1ULL;
		auto buffer = new char[bufferSize];
		sprintf_s(buffer, bufferSize, format, args ...);
		UE_LOG(LogTemp, Log, TEXT("%S"), buffer);
		delete buffer;
	}

	template<typename ... Args>
	void PrintLog(const wchar_t* const format, Args ... args)
	{
		const auto bufferSize = snwprintf(nullptr, 0, format, args ...) + 1ULL;
		auto buffer = new wchar_t[bufferSize];
		swprintf_s(buffer, bufferSize, format, args ...);
		UE_LOG(LogTemp, Log, TEXT("%S"), buffer);
		delete buffer;
	}
};
