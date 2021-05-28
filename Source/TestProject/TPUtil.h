// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TSingleton.h"

class TPUtil : public TSingleton<TPUtil>
{
public:	
	void CharToWChar(wchar_t* buffer, size_t bufferSize, const char* const source);
	void WCharToChar(char* buffer, size_t bufferSize, const wchar_t* const source);
	void WCharToMultiByte(char* buffer, size_t bufferSize, const wchar_t* const source);
	void MultiByteToWChar(wchar_t* buffer, size_t bufferSize, const char* const source);
};

struct Vector3
{
	float x;
	float y;
	float z;
};