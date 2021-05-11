// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TSingleton.h"
#include <vector>
#include <wchar.h>

using namespace std;

/**
 * 
 */
class TESTPROJECT_API TPError : public TSingleton<TPError>
{
public:
	void PrintError(const wchar_t* message);
	void PrintError(const wchar_t* message, const int code);
	void PrintErrors();
private:
	const wchar_t* ConcatMessage(const wchar_t* message, const int code);
};
