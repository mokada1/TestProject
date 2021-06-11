#pragma once

#include "../TP_generated.h"
#include "../Util/TPUtil.h"
#include "UObject/Object.h"
#include "TPObject.generated.h"

using namespace std;

UCLASS(BlueprintType)
class TESTPROJECT_API UTPObject : public UObject
{
	GENERATED_BODY()

public:
	virtual wchar_t* GetObjectId() const
	{
		return nullptr;
	}
	virtual bool IsValid() const
	{
		return false;
	}
	virtual ~UTPObject()
	{
	}
};