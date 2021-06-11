// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TSingleton.h"
#include <wchar.h>

class TESTPROJECT_API TPError : public TSingleton<TPError>
{
public:
	void PrintError(const wchar_t* const message);
	void PrintError(const wchar_t* const message, const int code);

private:
	const wchar_t* ConcatMessage(const wchar_t* const message, const int code);
};
